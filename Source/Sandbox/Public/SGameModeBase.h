// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SCoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class ISStoreObjsInOrder;
/**
 * 
 */
UCLASS()
class SANDBOX_API ASGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASGameModeBase();
    FOnMatchStateChangedSignature OnGameStateChanged;
    
    void StartGame();
    void QuitGame();

    TArray<TScriptInterface<ISStoredInOrder>> GetNeededOrderOfItems() { return NeededOrderOfItems; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    int32 NumberOfOrderedItems = 3;

private:
    ESGameState MatchState = ESGameState::WaitingToStart;
    void SetGameState(ESGameState State);

    UPROPERTY()
    TArray<TScriptInterface<ISStoredInOrder>> NeededOrderOfItems;
    TScriptInterface<ISStoreObjsInOrder> StoreObjsInOrder;
    UFUNCTION()
    void OnOrderOfItemsWasChanged(const TArray<TScriptInterface<ISStoredInOrder>> ArrayOfItemsInRightOrder);
    bool AreArraysEquivalent(const TArray<TScriptInterface<ISStoredInOrder>>& Array1, const TArray<TScriptInterface<ISStoredInOrder>>& Array2);
void SaveGameResults(FSGameResultInfo GameResults);
    FDateTime StartGameTime;
};
