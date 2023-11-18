// Copyright to Kat Code Labs, SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PortalComponents/OcclusionPortal.h"
#include "PortalsFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PORTALS_API UPortalsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static uint8 GetCurrentWorld(const TArray<uint8>& TraversedWorlds);

	UFUNCTION(BlueprintCallable)
	static void Traverse(const uint8 World, TArray<uint8>& TraversedWorlds);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float PortalDotProduct(const UOcclusionPortal* InPortal, const FVector& Location);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float PortalDotProduct_Actor(const UOcclusionPortal* InPortal, const AActor* InActor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float PortalDotProduct_Component(const UOcclusionPortal* InPortal, const USceneComponent* InComponent);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static UOcclusionPortalsSubsystem* GetOcclusionPortalsSubsystem(const UObject* WorldContextObject, int32 TrackedPlayerIndex = 0);
};
