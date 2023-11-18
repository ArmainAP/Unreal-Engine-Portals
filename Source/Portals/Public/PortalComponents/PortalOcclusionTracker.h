// Copyright to Kat Code Labs, SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OcclusionPortal.h"
#include "PortalTransitionInterface.h"
#include "Components/ActorComponent.h"
#include "PortalOcclusionTracker.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTALS_API UPortalOcclusionTracker : public UActorComponent, public IPortalTransitionInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPortalOcclusionTracker();
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnPortalBeginOverlap_Implementation(UOcclusionPortal* Portal, UPrimitiveComponent* Component, int32 BodyIndex, bool bFromSweep, const FHitResult& HitResult) override;
	virtual void OnPortalEndOverlap_Implementation(UOcclusionPortal* Portal, UPrimitiveComponent* Component, int32 BodyIndex) override;

	void TracePortals();
	void PerformVisibilityCheck(const UOcclusionPortal* Portal);
	void UpdateVisibility(const bool bNewVisibility);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FBeginOverlap, UOcclusionPortal*, Portal, UPrimitiveComponent*, Component, int32, BodyIndex, bool, bFromSweep, const FHitResult&, HitResult);
	UPROPERTY(BlueprintAssignable, Category = "Occlusion Portals Event")
	FBeginOverlap OnBeginOverlap;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEndOverlap, UOcclusionPortal*, Portal, UPrimitiveComponent*, Component, int32, BodyIndex);
	UPROPERTY(BlueprintAssignable, Category = "Occlusion Portals Event")
	FEndOverlap OnEndOverlap;

	// The player used for reference in all view calculations
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Occlusion Portals")
	int32 TrackedPlayerIndex = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Occlusion Portals")
	FName WorldParameterName = "World";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Occlusion Portals")
	FName OpacityOverrideName;


protected:
	// 0-255 is a stencil buffer limitation
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Occlusion Portals", meta=(UIMin = 0, ClampMin = 0, ClampMax = 255, UIMax = 255))
	uint8 World = 0;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<uint8> TraversedWorlds;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<UPrimitiveComponent*> TransitioningComponents;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float CachedVisibility;
};
