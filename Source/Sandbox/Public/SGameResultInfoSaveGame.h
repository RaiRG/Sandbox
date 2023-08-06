// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCoreTypes.h"
#include "GameFramework/SaveGame.h"
#include "SGameResultInfoSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOX_API USGameResultInfoSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
    UPROPERTY()
    TArray<FSGameResultInfo> AllGamesResultInfo;
};
