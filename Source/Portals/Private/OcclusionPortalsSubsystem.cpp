// Copyright to Kat Code Labs, SRL. All Rights Reserved.

#include "OcclusionPortalsSubsystem.h"

#include "PortalsFunctionLibrary.h"
#include "PortalComponents/MaskedOcclusionPortal.h"
#include "PortalComponents/OcclusionPortal.h"

bool UOcclusionPortalsSubsystem::ActivatePortal(UOcclusionPortal* OcclusionPortal)
{
	bool bReturn = false;
	if(!IsValid(OcclusionPortal))
	{
		return bReturn;
	}
	
	ActivePortals.Add(OcclusionPortal);
	bReturn = true;

	const UMaskedOcclusionPortal* MaskedOcclusionPortal = Cast<UMaskedOcclusionPortal>(OcclusionPortal);
	if(!IsValid(MaskedOcclusionPortal))
	{
		return bReturn;
	}
	
	// Virtual shadows are not supported for Masked Occlusion. 
	if(ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		LocalPlayer->ConsoleCommand("r.Shadow.Virtual.Enable 0");	
	}

	for (UMaskedOcclusionPortal* ActiveMaskedOcclusionPortal : ActiveMaskedPortals)
	{
		if (ActiveMaskedOcclusionPortal->GetWorld() == MaskedOcclusionPortal->GetWorld())
		{
			if(DeactivatePortal(ActiveMaskedOcclusionPortal))
			{
				bReturn = true;
			}
		}
	}
	
	return bReturn;
}

bool UOcclusionPortalsSubsystem::DeactivatePortal(UOcclusionPortal* OcclusionPortal)
{
	bool bReturn = false;
	if(!IsValid(OcclusionPortal))
	{
		return bReturn;
	}

	bReturn = ActivePortals.Contains(OcclusionPortal);
	if(bReturn)
	{
		bReturn = bReturn || ActivePortals.Contains(OcclusionPortal);
		if(bReturn)
		{
			ActivePortals.Remove(OcclusionPortal);
		}
	}
	
	if (const UMaskedOcclusionPortal* MaskedOcclusionPortal = Cast<UMaskedOcclusionPortal>(OcclusionPortal))
	{
		bReturn = bReturn || ActiveMaskedPortals.Contains(MaskedOcclusionPortal);
		if(bReturn)
		{
			ActiveMaskedPortals.Remove(MaskedOcclusionPortal);
		}
	}
	
	return bReturn;
}

void UOcclusionPortalsSubsystem::TransitPlayer(UOcclusionPortal* Portal)
{
	if(!IsValid(Portal))
	{
		return;
	}

	UPortalsFunctionLibrary::Traverse(Portal->GetPortalWorld(), PlayerWorlds);
	OnPlayerTransition.Broadcast(Portal);
}