// Copyright to Kat Code Labs, SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PortalTransitionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPortalTransitionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PORTALS_API IPortalTransitionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnPortalBeginOverlap(UOcclusionPortal* Portal, UPrimitiveComponent* Component, int32 BodyIndex, bool bFromSweep, const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnPortalEndOverlap(UOcclusionPortal* Portal, UPrimitiveComponent* Component, int32 BodyIndex);
};
