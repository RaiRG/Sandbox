// Copyright Epic Games, Inc. All Rights Reserved.


#include "SGameModeBase.h"

#include "SVrPawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogSGameModeBase, All, All);

ASGameModeBase::ASGameModeBase()
{
    DefaultPawnClass = ASVrPawn::StaticClass();
}

void ASGameModeBase::StartGame()
{
    SetGameState(ESGameState::InProgress);
}

void ASGameModeBase::QuitGame()
{
    FPlatformMisc::RequestExit(false);
}

void ASGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    SetGameState(ESGameState::WaitingToStart);
}

void ASGameModeBase::SetGameState(ESGameState State)
{
    if (MatchState == State)
        return;

    MatchState = State;

    UE_LOG(LogSGameModeBase, Display, TEXT("OnGameStateChanged Broadcast"));

    OnGameStateChanged.Broadcast(MatchState);
}
