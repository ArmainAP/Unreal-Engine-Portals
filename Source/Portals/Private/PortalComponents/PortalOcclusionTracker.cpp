// Copyright to Kat Code Labs, SRL. All Rights Reserved.


#include "PortalComponents/PortalOcclusionTracker.h"

#include "OcclusionPortalsSubsystem.h"
#include "PortalsFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UPortalOcclusionTracker::UPortalOcclusionTracker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPortalOcclusionTracker::BeginPlay()
{
	Super::BeginPlay();

	if(World)
	{
		TraversedWorlds.Add(World);
	}
}


// Called every frame
void UPortalOcclusionTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TracePortals();
	
	if(const AActor* Owner = GetOwner())
	{
		TArray<UMeshComponent*> MeshComponents;
		Owner->GetComponents<UMeshComponent*>(MeshComponents);
		for (UMeshComponent* MeshComponent : MeshComponents)
		{
			const uint8 CurrentWorld = UPortalsFunctionLibrary::GetCurrentWorld(TraversedWorlds);
			MeshComponent->SetScalarParameterValueOnMaterials(WorldParameterName, CurrentWorld);
		}
	}
}

void UPortalOcclusionTracker::OnPortalBeginOverlap_Implementation(UOcclusionPortal* Portal, UPrimitiveComponent* Component,
	int32 BodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	if(UMeshComponent* MeshComponent = Cast<UMeshComponent>(Component))
	{
		MeshComponent->SetScalarParameterValueOnMaterials(OpacityOverrideName, CachedVisibility);
	}
	
	TransitioningComponents.Add(Component);
	OnBeginOverlap.Broadcast(Portal, Component, BodyIndex, bFromSweep, HitResult);
}

void UPortalOcclusionTracker::OnPortalEndOverlap_Implementation(UOcclusionPortal* Portal, UPrimitiveComponent* Component,
	int32 BodyIndex)
{
	if(UMeshComponent* MeshComponent = Cast<UMeshComponent>(Component))
	{
		MeshComponent->SetScalarParameterValueOnMaterials(OpacityOverrideName, 0.5f);
	}
	
	TransitioningComponents.Remove(Component);
	if(!TransitioningComponents.Num())
	{
		UPortalsFunctionLibrary::Traverse(Portal->GetPortalWorld(), TraversedWorlds);
	}
	
	OnEndOverlap.Broadcast(Portal, Component, BodyIndex);
}

void UPortalOcclusionTracker::TracePortals()
{
	const AActor* Owner = GetOwner();
	if(!IsValid(Owner))
	{
		return;
	}
	
	const APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, TrackedPlayerIndex);
	if(!IsValid(PlayerCameraManager))
	{
		return;
	}
	
	const FVector Start = Owner->GetActorLocation();
	const FVector End = PlayerCameraManager->GetCameraLocation();
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::LineTraceMultiForObjects(this, Start, End, {UEngineTypes::ConvertToObjectType(ECC_WorldDynamic)}, false, {}, EDrawDebugTrace::None, HitResults, true);

	bool bWasObservedTroughPortal = false;
	for(FHitResult& HitResult : HitResults)
	{
		if(const UOcclusionPortal* Portal = Cast<UOcclusionPortal>(HitResult.GetComponent()))
		{
			bWasObservedTroughPortal = true;
			PerformVisibilityCheck(Portal);
		}
	}

	if(!bWasObservedTroughPortal)
	{
		PerformVisibilityCheck(nullptr);
	}
}

void UPortalOcclusionTracker::PerformVisibilityCheck(const UOcclusionPortal* Portal)
{
	const UOcclusionPortalsSubsystem* PortalsSubsystem = UPortalsFunctionLibrary::GetOcclusionPortalsSubsystem(this, TrackedPlayerIndex);
	if(!IsValid(PortalsSubsystem))
	{
		return;
	}
	
	const uint8 CurrentObjectWorld = UPortalsFunctionLibrary::GetCurrentWorld(TraversedWorlds);
	const uint8 CurrentPlayerWorld = UPortalsFunctionLibrary::GetCurrentWorld(PortalsSubsystem->PlayerWorlds);
	const bool bIsSameWorldAsPlayer = CurrentObjectWorld == CurrentPlayerWorld;

	// Is observed trough a portal?
	if (IsValid(Portal))
	{
		// Then the object and portal must be of the same world but in a different world from the player.
		const bool bIsSameWorldAsPortal = Portal->GetPortalWorld() == CurrentObjectWorld;
		UpdateVisibility(bIsSameWorldAsPortal && !bIsSameWorldAsPlayer);
	}
	else // Is it observed directly?
	{
		// Is it in the same world as the player?
		UpdateVisibility(bIsSameWorldAsPlayer);
	}
}

void UPortalOcclusionTracker::UpdateVisibility(const bool bNewVisibility)
{
	CachedVisibility = bNewVisibility;
	if(const AActor* Owner = GetOwner())
	{
		TArray<UPrimitiveComponent*> MeshComponents;
		Owner->GetComponents<UPrimitiveComponent*>(MeshComponents);
		for (UPrimitiveComponent* PrimitiveComponent : MeshComponents)
		{
			const bool bIsTransitioning = TransitioningComponents.Contains(PrimitiveComponent);
			const ECollisionEnabled::Type NewCollision = CachedVisibility || bIsTransitioning ?
				ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;
			PrimitiveComponent->SetCollisionEnabled(NewCollision);
		}
	}
}
