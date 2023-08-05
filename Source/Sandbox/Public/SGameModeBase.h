// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SCoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

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

protected:
    virtual void BeginPlay() override;

private:
    ESGameState MatchState = ESGameState::WaitingToStart;
    void SetGameState(ESGameState State);

};
