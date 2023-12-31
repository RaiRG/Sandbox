// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SAllGamesResultsWidget.h"

#include "SGameInstance.h"
#include "SGameModeBase.h"
#include "SOneGameResultInfoWidget.h"
#include "Components/VerticalBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogSAllGamesResultsWidget, All, All);


void USAllGamesResultsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    FillWithInfoAboutPreviousGames();

    if (auto GameMode = Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        GameMode->OnGameStateChanged.AddUObject(this, &USAllGamesResultsWidget::OnGameStateChanged);
    }
}

void USAllGamesResultsWidget::FillWithInfoAboutPreviousGames()
{
    if (AllGamesListVerticalBox)
    {
        AllGamesListVerticalBox->ClearChildren();
    }
    if (auto GameInstance = Cast<USGameInstance>(GetWorld()->GetGameInstance()))
    {
        auto AllGamesInfo = GameInstance->LoadAllGamesInfo();
        
        int32 AddedWidgetNumber = 0;
        for (int i=AllGamesInfo.Num()-1; i>=0; i--)
        {
            if (AddedWidgetNumber<MaxNumberOfShowedResults)
            {
                auto GameInfo = AllGamesInfo[i];
                UE_LOG(LogSAllGamesResultsWidget, Display, TEXT("Founded game!"))
                if (auto Widget = Cast<USOneGameResultInfoWidget>(CreateWidget(this, OneGameResultInfoWidgetClass)))
                {
                    Widget->SetInfo(GameInfo);
                    AllGamesListVerticalBox->AddChild(Widget);
                    AddedWidgetNumber++;
                }
            }
        }
    }
}

void USAllGamesResultsWidget::OnGameStateChanged(ESGameState GameState)
{
    if (GameState == ESGameState::End)
    {
        FillWithInfoAboutPreviousGames();
    }
}
