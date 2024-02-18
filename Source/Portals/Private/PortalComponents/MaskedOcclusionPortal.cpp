// Copyright to Kat Code Labs, SRL. All Rights Reserved.


#include "PortalComponents/MaskedOcclusionPortal.h"

#include "OcclusionPortalsSubsystem.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UMaskedOcclusionPortal::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!IsActive())
	{
		return;
	}
	
	UKismetMaterialLibrary::SetVectorParameterValue(this, MaskedPortalParameters.MaskParameters, "A", UKismetMathLibrary::Conv_VectorToLinearColor(GetSocketLocation("A")));
	UKismetMaterialLibrary::SetVectorParameterValue(this, MaskedPortalParameters.MaskParameters, "B", UKismetMathLibrary::Conv_VectorToLinearColor(GetSocketLocation("B")));
	UKismetMaterialLibrary::SetVectorParameterValue(this, MaskedPortalParameters.MaskParameters, "C", UKismetMathLibrary::Conv_VectorToLinearColor(GetSocketLocation("C")));
	UKismetMaterialLibrary::SetVectorParameterValue(this, MaskedPortalParameters.MaskParameters, "D", UKismetMathLibrary::Conv_VectorToLinearColor(GetSocketLocation("D")));
}
