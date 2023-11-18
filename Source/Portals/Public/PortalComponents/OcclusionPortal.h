// Copyright to Kat Code Labs, SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "OcclusionPortal.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class PORTALS_API UOcclusionPortal : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UOcclusionPortal();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION()
	void OnPortalBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPortalEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UFUNCTION(BlueprintCallable, Category="Occlusion Portal")
	void SetPortalWorld(const uint8& NewWorld);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Occlusion Portal")
	FORCEINLINE uint8 GetPortalWorld() const
	{
		return PortalWorld;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Occlusion Portal")
	float GetComputedPlayerDotProduct() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Occlusion Portal")
	FORCEINLINE float GetCachedPlayerDotProduct() const
	{
		return CachedPlayerDotProduct;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Occlusion Portal")
	UOcclusionPortalsSubsystem* GetOcclusionPortalSubsystem() const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void PlayerTransition();

public:
	// The player used for reference in all view calculations
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Occlusion Portal")
	int32 TrackedPlayerIndex = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = "Occlusion Portal")
	bool CachedIsBackside = false;

protected:
	// 0-255 is a stencil buffer limitation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, BlueprintSetter=SetPortalWorld, BlueprintGetter=GetPortalWorld, Category = "Occlusion Portal", meta=(UIMin = 0, ClampMin = 0, ClampMax = 255, UIMax = 255))
	uint8 PortalWorld = 0;
	
	UPROPERTY(BlueprintReadOnly, BlueprintGetter=GetCachedPlayerDotProduct, Category = "Occlusion Portal")
	float CachedPlayerDotProduct = 0.0f;
};
