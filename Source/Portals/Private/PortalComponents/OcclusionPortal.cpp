// Copyright to Kat Code Labs, SRL. All Rights Reserved.


#include "PortalComponents/OcclusionPortal.h"

#include "OcclusionPortalsSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "PortalComponents/PortalOcclusionTracker.h"

UOcclusionPortal::UOcclusionPortal()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Script/Engine.Material'/Portals/Materials/M_TranslucentOcclusion.M_TranslucentOcclusion'"));
	if (MaterialAsset.Succeeded())
	{
		UMeshComponent::SetMaterial(0, MaterialAsset.Object);
	}
	
	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    UPrimitiveComponent::SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
	bRenderInMainPass = false;
	bRenderCustomDepth = true;
	SetPortalWorld(PortalWorld);

	PrimaryComponentTick.bCanEverTick = true;

	OnComponentBeginOverlap.AddDynamic(this, &UOcclusionPortal::OnPortalBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UOcclusionPortal::OnPortalEndOverlap);
}

#if WITH_EDITOR
void UOcclusionPortal::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UOcclusionPortal, PortalWorld))
	{
		SetPortalWorld(PortalWorld);
	}
}
#endif

void UOcclusionPortal::Activate(bool bReset)
{
	UOcclusionPortalsSubsystem* OcclusionPortalsSubsystem = GetOcclusionPortalSubsystem();
	if(!IsValid(OcclusionPortalsSubsystem))
	{
		return;
	}

	if (OcclusionPortalsSubsystem->ActivatePortal(this))
	{
		Super::Activate(bReset);
	}
}

void UOcclusionPortal::Deactivate()
{
	Super::Deactivate();
}

void UOcclusionPortal::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bAutoActivate && !IsActive())
	{
		bAutoActivate = false;
		Activate(false);
	}

	if(!IsActive())
	{
		return;
	}

	CachedPlayerDotProduct = GetComputedPlayerDotProduct();
	const bool IsBackside = FMath::RoundFromZero(CachedPlayerDotProduct) > 0;
	if (CachedIsBackside != IsBackside)
	{
		CachedIsBackside = IsBackside;
		if(IsOverlappingActor(UGameplayStatics::GetPlayerPawn(this, TrackedPlayerIndex)))
		{
			PlayerTransition();
		}
	}
}

void UOcclusionPortal::OnPortalBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsActive())
	{
		return;
	}
		
	if(IsValid(OtherComp) && OtherComp->Implements<UPortalTransitionInterface>())
	{
		IPortalTransitionInterface::Execute_OnPortalBeginOverlap(OtherComp, this, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}

	if(!IsValid(OtherActor))
	{
		return;
	}

	if(OtherActor->Implements<UPortalTransitionInterface>())
	{
		IPortalTransitionInterface::Execute_OnPortalBeginOverlap(OtherActor, this, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
	
	if(UPortalOcclusionTracker* PortalOcclusionTracker = OtherActor->GetComponentByClass<UPortalOcclusionTracker>())
	{
		IPortalTransitionInterface::Execute_OnPortalBeginOverlap(PortalOcclusionTracker, this, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

void UOcclusionPortal::OnPortalEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!IsActive())
	{
		return;
	}
	
	if(IsValid(OtherComp) && OtherComp->Implements<UPortalTransitionInterface>())
	{
		IPortalTransitionInterface::Execute_OnPortalEndOverlap(OtherComp, this, OtherComp, OtherBodyIndex);
	}

	if(!IsValid(OtherActor))
	{
		return;
	}

	if(OtherActor->Implements<UPortalTransitionInterface>())
	{
		IPortalTransitionInterface::Execute_OnPortalEndOverlap(OtherActor, this, OtherComp, OtherBodyIndex);
	}

	if(UPortalOcclusionTracker* PortalOcclusionTracker = OtherActor->GetComponentByClass<UPortalOcclusionTracker>())
	{
		IPortalTransitionInterface::Execute_OnPortalEndOverlap(PortalOcclusionTracker, this, OtherComp, OtherBodyIndex);
	}
}

void UOcclusionPortal::SetPortalWorld(const uint8& NewWorld)
{
	PortalWorld = FMath::Clamp(NewWorld, 0, 255);
	SetCustomDepthStencilValue(PortalWorld);
}

float UOcclusionPortal::GetComputedPlayerDotProduct() const
{
	if(const APlayerCameraManager* TrackedPlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, TrackedPlayerIndex))
	{
		return UPortalsFunctionLibrary::PortalDotProduct(this, TrackedPlayerCameraManager->GetCameraLocation());	
	}
	return 0.0f;
}

UOcclusionPortalsSubsystem* UOcclusionPortal::GetOcclusionPortalSubsystem() const
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, TrackedPlayerIndex);
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}
	
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		return nullptr;
	}
		
	return LocalPlayer->GetSubsystem<UOcclusionPortalsSubsystem>();
}

void UOcclusionPortal::PlayerTransition_Implementation()
{
	if(UOcclusionPortalsSubsystem* OcclusionPortalsSubsystem = GetOcclusionPortalSubsystem())
	{
		OcclusionPortalsSubsystem->TransitPlayer(this);
	}
}
