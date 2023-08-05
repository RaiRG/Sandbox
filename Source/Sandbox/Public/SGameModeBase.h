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
    FOnMatchStateChangedSignature OnMatchStateChanged;

    void StartGame();
    void QuitGame();

private:
    ESTGameState MatchState = ESTGameState::WaitingToStart;
    
    void SetMatchState(ESTGameState State);

};
