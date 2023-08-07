// Copyright Epic Games, Inc. All Rights Reserved.


#include "SGameModeBase.h"
#include "SGameInstance.h"
#include "SStoreObjsInOrder.h"
#include "SVrPawn.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSGameModeBase, All, All);

ASGameModeBase::ASGameModeBase()
{
    DefaultPawnClass = ASVrPawn::StaticClass();
}


void ASGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    SetGameState(ESGameState::WaitingToStart);

    TArray<AActor*> StoredInOrderActors;
    UGameplayStatics::GetAllActorsWithInterface(this, USStoredInOrder::StaticClass(), StoredInOrderActors);
    if (StoredInOrderActors.Num() < NumberOfOrderedItems)
    {
        UE_LOG(LogSGameModeBase, Error, TEXT("Number of ISStoredInOrder is less than need"));
    }
    else
    {
        while (NeededOrderOfItems.Num() < NumberOfOrderedItems)
        {
            const auto RandomIndex = FMath::RandRange(0, StoredInOrderActors.Num() - 1);
            if (!NeededOrderOfItems.Contains(TScriptInterface<ISStoredInOrder>(StoredInOrderActors[RandomIndex])))
            {
                NeededOrderOfItems.Add(TScriptInterface<ISStoredInOrder>(StoredInOrderActors[RandomIndex]));
            }
        }
    }
}

void ASGameModeBase::StartGame()
{
    StartGameTime = FDateTime::Now();
    TArray<AActor*> ContainerActors;
    UGameplayStatics::GetAllActorsWithInterface(this, USStoreObjsInOrder::StaticClass(), ContainerActors);
    if (ContainerActors.Num() > 1)
    {
        UE_LOG(LogSGameModeBase, Warning, TEXT("Number of ISStoreObjsInOrder is more than 1"));
    }
    if (ContainerActors.Num() != 0)
    {
        auto Actor = ContainerActors[0];
        StoreObjsInOrder = TScriptInterface<ISStoreObjsInOrder>(Actor);
        if (StoreObjsInOrder)
        {
            StoreObjsInOrder->GetOnOrderOfObjsChangedSignature().AddDynamic(this, &ASGameModeBase::OnOrderOfItemsWasChanged);
        }
    }
    else
    {
        UE_LOG(LogSGameModeBase, Warning, TEXT("Number of ISStoreObjsInOrder is 0"));
    }

    SetGameState(ESGameState::InProgress);
}

void ASGameModeBase::QuitGame()
{
    //TODO: remove comments
    //FPlatformMisc::RequestExit(false);
}


void ASGameModeBase::SetGameState(ESGameState State)
{
    if (MatchState == State)
        return;

    MatchState = State;

    UE_LOG(LogSGameModeBase, Display, TEXT("OnGameStateChanged Broadcast"));

    OnGameStateChanged.Broadcast(MatchState);
}

void ASGameModeBase::OnOrderOfItemsWasChanged(const TArray<TScriptInterface<ISStoredInOrder>> ArrayOfItemsInRightOrder)
{
    const auto IsEqual = AreArraysEquivalent(ArrayOfItemsInRightOrder, NeededOrderOfItems);

    if (!IsEqual)
        return;

    UE_LOG(LogSGameModeBase, Display, TEXT("All items are ordered"));
    if (StoreObjsInOrder)
    {
        StoreObjsInOrder->GetOnOrderOfObjsChangedSignature().RemoveDynamic(this, &ASGameModeBase::OnOrderOfItemsWasChanged);
    }

    FSGameResultInfo PlayerResults;
    PlayerResults.TimeAtTheEnd = FDateTime::Now();
    PlayerResults.TotalTime = PlayerResults.TimeAtTheEnd - StartGameTime;
    SaveGameResults(PlayerResults);
    SetGameState(ESGameState::End);
}


bool ASGameModeBase::AreArraysEquivalent(const TArray<TScriptInterface<ISStoredInOrder>>& Array1,
    const TArray<TScriptInterface<ISStoredInOrder>>& Array2)
{
    if (Array1.Num() != Array2.Num())
    {
        return false;
    }

    for (int32 Index = 0; Index < Array1.Num(); ++Index)
    {
        if (Array1[Index] != Array2[Index])
        {
            return false;
        }
    }

    return true;
}

void ASGameModeBase::SaveGameResults(FSGameResultInfo PlayerResults)
{      
    if (auto GameInstance = Cast<USGameInstance>(GetWorld()->GetGameInstance()))
    {
        GameInstance->SaveGameResults(PlayerResults);
    }
}
