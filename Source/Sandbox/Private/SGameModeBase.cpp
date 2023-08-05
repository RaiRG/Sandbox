// Copyright Epic Games, Inc. All Rights Reserved.


#include "SGameModeBase.h"

void ASGameModeBase::StartGame()
{
    SetMatchState(ESTGameState::InProgress);
}

void ASGameModeBase::QuitGame()
{
    FPlatformMisc::RequestExit(false);
}

void ASGameModeBase::SetMatchState(ESTGameState State)
{
    if (MatchState == State)
        return;

    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}
