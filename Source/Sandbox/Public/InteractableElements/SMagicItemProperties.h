// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SMagicItemProperties.generated.h"

class USoundBase;

UCLASS()
class SANDBOX_API USMagicItemProperties : public UDataAsset
{
	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    USoundBase* PickUpSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    USoundBase* DropSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor MagicTrackColor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UStaticMesh* Appearance;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UTexture2D* Icon;
};
