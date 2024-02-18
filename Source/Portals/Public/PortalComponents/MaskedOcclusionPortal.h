// Copyright to Kat Code Labs, SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OcclusionPortal.h"
#include "Data/MaskedPortalParameters.h"
#include "MaskedOcclusionPortal.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class PORTALS_API UMaskedOcclusionPortal : public UOcclusionPortal
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaskedPortalParameters MaskedPortalParameters;
};
