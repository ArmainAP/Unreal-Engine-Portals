// Copyright to Kat Code Labs, SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PortalsFunctionLibrary.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "OcclusionPortalsSubsystem.generated.h"

class UMaskedOcclusionPortal;
class UOcclusionPortal;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PORTALS_API UOcclusionPortalsSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool ActivatePortal(UOcclusionPortal* OcclusionPortal);

	UFUNCTION(BlueprintCallable)
	bool DeactivatePortal(UOcclusionPortal* OcclusionPortal);

	UFUNCTION(BlueprintCallable)
	void TransitPlayer(UOcclusionPortal* Portal);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE uint8 GetPlayerWorld()
	{
		return UPortalsFunctionLibrary::GetCurrentWorld(PlayerWorlds);
	}

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TSet<UOcclusionPortal*> ActivePortals;

	// Only one masked portal can be enabled at any given time. 
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TSet<UMaskedOcclusionPortal*> ActiveMaskedPortals;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<uint8> PlayerWorlds;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTransitionDelegate, UOcclusionPortal*, Portal);
	UPROPERTY(BlueprintAssignable, Category = "Occlusion Portal Event")
	FTransitionDelegate OnPlayerTransition;
};
