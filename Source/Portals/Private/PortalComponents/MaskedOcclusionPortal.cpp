// Copyright to Kat Code Labs, SRL. All Rights Reserved.


#include "PortalComponents/MaskedOcclusionPortal.h"

#include "OcclusionPortalsSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UMaskedOcclusionPortal::GetSocketTransformations(const FName& Socket, FVector& SocketLocation, FVector& SocketRotationVector, const bool bUseRightVector) const
{
	SocketLocation = GetSocketLocation(Socket);
	SocketRotationVector = bUseRightVector ? UKismetMathLibrary::GetRightVector(GetSocketRotation(Socket)) : UKismetMathLibrary::GetUpVector(GetSocketRotation(Socket));
}

FVector UMaskedOcclusionPortal::CalculateRotationVector(const FVector& CameraLocation, const FVector& SocketLocation, const FVector& SocketRotationVector, const bool bIsBackside, const bool bIsXY)
{
	const FVector Delta = bIsBackside ? CameraLocation - SocketLocation : SocketLocation - CameraLocation;

	return bIsXY ? UKismetMathLibrary::GetUpVector(UKismetMathLibrary::MakeRotFromXY(Delta, SocketRotationVector))
		: UKismetMathLibrary::GetRightVector(UKismetMathLibrary::MakeRotFromXZ(Delta, SocketRotationVector));
}

void UMaskedOcclusionPortal::SetMaskedPortalParameters(const FVector& RotationVector, const FVector& SocketLocation, const FString& StartParameterName, const FString& FinishParameterName)
{
	const FVector Start = RotationVector + SocketLocation;
	const FVector End = RotationVector + Start;
	UKismetMaterialLibrary::SetVectorParameterValue(this, MaskedPortalParameters.MaskParameters, FName(*StartParameterName), UKismetMathLibrary::Conv_VectorToLinearColor(Start));
	UKismetMaterialLibrary::SetVectorParameterValue(this, MaskedPortalParameters.MaskParameters, FName(*FinishParameterName), UKismetMathLibrary::Conv_VectorToLinearColor(End));
}

void UMaskedOcclusionPortal::CalculateMaskedPortalParameters(const FName& Socket, const FVector& CameraLocation, bool bIsBackside, const FString& StartParameterName, const FString& FinishParameterName, float Direction)
{
	FVector SocketLocation, SocketRotationVector;
	GetSocketTransformations(Socket, SocketLocation, SocketRotationVector, true);
	const FVector RotationVector = CalculateRotationVector(CameraLocation, SocketLocation, SocketRotationVector, bIsBackside, true);
	SetMaskedPortalParameters(RotationVector * RenderDistance * Direction, SocketLocation, StartParameterName, FinishParameterName);
}

void UMaskedOcclusionPortal::CalculateMaskedPortalParametersSide(const FName& Socket, const FVector& CameraLocation, bool bIsBackside, const FString& StartParameterName, const FString& FinishParameterName, float Direction)
{
	Direction *= bIsBackside ? 1 : -1; 
	FVector SocketLocation, SocketRotationVector;
	GetSocketTransformations(Socket, SocketLocation, SocketRotationVector, false);
	const FVector RotationVector = CalculateRotationVector(CameraLocation, SocketLocation, SocketRotationVector);
	SetMaskedPortalParameters(RotationVector * RenderDistance * Direction, SocketLocation, StartParameterName, FinishParameterName);
}

void UMaskedOcclusionPortal::CalculateMaskedPortalParametersBackSide(bool bIsBackside, const FString& StartParameterName, const FString& FinishParameterName)
{
	const float Multiplier = bIsBackside ? 1 : -1;
	const FVector Location = GetComponentLocation();
	const FVector Forward = GetForwardVector();
	SetMaskedPortalParameters(Forward * RenderDistance * Multiplier, Location, StartParameterName, FinishParameterName);
}

void UMaskedOcclusionPortal::PlayerTransition_Implementation()
{
	Super::PlayerTransition_Implementation();
}

void UMaskedOcclusionPortal::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!IsActive())
	{
		return;
	}
	
	UpdateMaskParameters();
}

void UMaskedOcclusionPortal::UpdateMaskParameters()
{
	const APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, TrackedPlayerIndex);
	if(!IsValid(PlayerCameraManager))
	{
		return;
	}
	
	const FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
	CalculateMaskedPortalParametersBackSide(CachedIsBackside, MaskedPortalParameters.BackStartParameterName + FString::FromInt(PortalWorld), MaskedPortalParameters.BackFinishParameterName + FString::FromInt(PortalWorld));
	CalculateMaskedPortalParameters("U", CameraLocation, CachedIsBackside, MaskedPortalParameters.UpStartParameterName + FString::FromInt(PortalWorld), MaskedPortalParameters.UpFinishParameterName + FString::FromInt(PortalWorld), 1.0f);
	CalculateMaskedPortalParameters("D", CameraLocation, CachedIsBackside, MaskedPortalParameters.BottomStartParameterName + FString::FromInt(PortalWorld), MaskedPortalParameters.BottomFinishParameterName + FString::FromInt(PortalWorld), -1.0f);
	CalculateMaskedPortalParametersSide("L", CameraLocation, CachedIsBackside, MaskedPortalParameters.LeftStartParameterName + FString::FromInt(PortalWorld), MaskedPortalParameters.LeftFinishParameterName + FString::FromInt(PortalWorld), 1.0f);
	CalculateMaskedPortalParametersSide("R", CameraLocation, CachedIsBackside, MaskedPortalParameters.RightStartParameterName + FString::FromInt(PortalWorld), MaskedPortalParameters.RightFinishParameterName + FString::FromInt(PortalWorld), -1.0f);

	const FName ParameterName = FName(MaskedPortalParameters.SideParameterName + FString::FromInt(PortalWorld));
	const bool PlayerSide = GetOcclusionPortalSubsystem()->GetPlayerWorld() == PortalWorld;
	UKismetMaterialLibrary::SetScalarParameterValue(this, MaskedPortalParameters.MaskParameters, ParameterName, PlayerSide);
	UKismetMaterialLibrary::SetScalarParameterValue(this, MaskedPortalParameters.MaskParameters, "World", GetOcclusionPortalSubsystem()->GetPlayerWorld());
}
