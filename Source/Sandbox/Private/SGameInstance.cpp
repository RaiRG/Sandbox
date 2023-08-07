// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"

#include "SGameResultInfoSaveGame.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSGameInstance, All, All);


void USGameInstance::SaveGameResults(FSGameResultInfo GameResultForSaving)
{
    UE_LOG(LogSGameInstance, Display, TEXT("SaveGameResults"));
    USGameResultInfoSaveGame* SaveGameInstance;
    if (UGameplayStatics::DoesSaveGameExist(GameResultsSavingSlot, 0))
    {
        SaveGameInstance = Cast<USGameResultInfoSaveGame>(UGameplayStatics::LoadGameFromSlot(GameResultsSavingSlot, 0));
    }
    else
    {
        SaveGameInstance = Cast<USGameResultInfoSaveGame>(
            UGameplayStatics::CreateSaveGameObject(USGameResultInfoSaveGame::StaticClass()));
    }

    SaveGameInstance->AllGamesResultInfo.Add(GameResultForSaving);
    UGameplayStatics::SaveGameToSlot(SaveGameInstance, GameResultsSavingSlot, 0);

}

TArray<FSGameResultInfo> USGameInstance::LoadAllGamesInfo() const
{
    if (UGameplayStatics::DoesSaveGameExist(GameResultsSavingSlot, 0))
    {
        if (const auto SaveGameInstance = Cast<USGameResultInfoSaveGame>(UGameplayStatics::LoadGameFromSlot(GameResultsSavingSlot, 0)))
        {
            return SaveGameInstance->AllGamesResultInfo;
        }
    }
    return TArray<FSGameResultInfo>();
}