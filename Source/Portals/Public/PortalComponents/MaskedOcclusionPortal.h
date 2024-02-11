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
	
	UFUNCTION(BlueprintCallable)
	void UpdateMaskParameters();

protected:
	static FVector CalculateRotationVector(const FVector& CameraLocation, const FVector& SocketLocation, const FVector& SocketRotationVector, bool bIsBackside = false, bool bIsXY = false);
	void SetMaskedPortalParameters(const FVector& RotationVector, const FVector& SocketLocation,
	                               const FString& StartParameterName, const FString& FinishParameterName);

	void CalculateMaskedPortalParameters(const FName& Socket, const FVector& CameraLocation, bool bIsBackside,
	                                     const FString& StartParameterName, const FString& FinishParameterName, float Direction);
	void CalculateMaskedPortalParametersSide(const FName& Socket, const FVector& CameraLocation, bool bIsBackside,
	                                         const FString& StartParameterName, const FString& FinishParameterName,
	                                         float Direction);
	void CalculateMaskedPortalParametersBackSide(bool bIsBackside, const FString& StartParameterName, const FString& FinishParameterName);

	virtual void PlayerTransition_Implementation() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FMaskedPortalParameters MaskedPortalParameters;

	// Adjusts the rendering distance for portal visibility.
	// Consider reducing this value on less powerful systems like Oculus Quest.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RenderDistance = 100000.0f;
};
