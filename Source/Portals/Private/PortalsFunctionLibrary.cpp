// Copyright to Kat Code Labs, SRL. All Rights Reserved.

#include "PortalsFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

uint8 UPortalsFunctionLibrary::GetCurrentWorld(const TArray<uint8>& TraversedWorlds)
{
	if(TraversedWorlds.Num())
	{
		return TraversedWorlds.Last();
	}
	return 0;
}

void UPortalsFunctionLibrary::Traverse(const uint8 World, TArray<uint8>& TraversedWorlds)
{
	const uint8 CurrentWorld = GetCurrentWorld(TraversedWorlds);
	if(CurrentWorld == World)
	{
		TraversedWorlds.Pop();
	}
	else
	{
		TraversedWorlds.Add(World);
	}
}

float UPortalsFunctionLibrary::PortalDotProduct(const UOcclusionPortal* InPortal, const FVector& Location)
{
	if(IsValid(InPortal))
	{
		const FVector DeltaSafeNormal = (Location - InPortal->GetComponentLocation()).GetSafeNormal();
		return FVector::DotProduct(InPortal->GetForwardVector(), DeltaSafeNormal);
	}
	return 0;
}

float UPortalsFunctionLibrary::PortalDotProduct_Actor(const UOcclusionPortal* InPortal, const AActor* InActor)
{
	if(IsValid(InActor))
	{
		return PortalDotProduct(InPortal, InActor->GetActorLocation());
	}
	return 0;
}

float UPortalsFunctionLibrary::PortalDotProduct_Component(const UOcclusionPortal* InPortal,
	const USceneComponent* InComponent)
{
	if(IsValid(InComponent))
	{
		return PortalDotProduct(InPortal, InComponent->GetComponentLocation());
	}
	return 0;
}

UOcclusionPortalsSubsystem* UPortalsFunctionLibrary::GetOcclusionPortalsSubsystem(const UObject* WorldContextObject,
	int32 TrackedPlayerIndex)
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, TrackedPlayerIndex);
	if(!IsValid(PlayerController))
	{
		return nullptr;
	}

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if(!IsValid(LocalPlayer))
	{
		return nullptr;
	}

	return LocalPlayer->GetSubsystem<UOcclusionPortalsSubsystem>();
}
