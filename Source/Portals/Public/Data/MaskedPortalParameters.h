// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MaskedPortalParameters.generated.h"

USTRUCT(BlueprintType, BLueprintable)
struct FMaskedPortalParameters
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UMaterialParameterCollection* MaskParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SideParameterName = "Side";
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UpStartParameterName = "UpStart";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UpFinishParameterName = "UpFinish";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BottomStartParameterName = "BottomStart";
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BottomFinishParameterName = "BottomFinish";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BackStartParameterName = "BackStart";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BackFinishParameterName = "BackFinish";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LeftStartParameterName = "LeftStart";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LeftFinishParameterName = "LeftFinish";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RightStartParameterName = "RightStart";
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RightFinishParameterName = "RightFinish";
};