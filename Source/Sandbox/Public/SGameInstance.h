// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SCoreTypes.h"
#include "SGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SANDBOX_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    void SaveGameResults(FSGameResultInfo GameResultForSaving);
    TArray<FSGameResultInfo> LoadAllGamesInfo() const;
protected:

    UFUNCTION()
    void OnGameSaved(const FString& Str, const int32 Number, bool Success);
    
    UPROPERTY(EditDefaultsOnly, Category="Save Game")
    FString GameResultsSavingSlot = "GameResult";
};
