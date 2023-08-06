// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SMainMenuWidget.h"

#include "SAllGamesResultsWidget.h"
#include "SGameModeBase.h"
#include "SOrderGameWidget.h"
#include "Components/Button.h"

DEFINE_LOG_CATEGORY_STATIC(LogSMainMenuWidget, All, All);

void USMainMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &USMainMenuWidget::OnStartGame);
    }
    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &USMainMenuWidget::OnQuitGame);
    }
    if (ShowResultButton)
    {
        ShowResultButton->OnClicked.AddDynamic(this, &USMainMenuWidget::OnShowResults);
    }
    auto GameMode = GetGameMode();
    if (GameMode)
    {
        GameMode->OnGameStateChanged.AddUObject(this, &USMainMenuWidget::OnGameStateChanged);
    }
    ChangeVisibilityOfChildWidgets(ESlateVisibility::Hidden, ESlateVisibility::Hidden);

}

void USMainMenuWidget::OnStartGame()
{
    if (!GetGameMode())
        return;
    StartButton->SetIsEnabled(false);
    UE_LOG(LogSMainMenuWidget, Display, TEXT("OnStartGame"));
    GetGameMode()->StartGame();
}

void USMainMenuWidget::OnShowResults()
{
    if (AllGamesResultsWidget->GetVisibility() == ESlateVisibility::Hidden)
    {
        ChangeVisibilityOfChildWidgets(ESlateVisibility::Hidden, ESlateVisibility::Visible);
    }
    else
    {
        ChangeVisibilityOfChildWidgets(ESlateVisibility::Visible, ESlateVisibility::Hidden);

    }
}

void USMainMenuWidget::OnQuitGame()
{
    if (!GetGameMode())
        return;
    GetGameMode()->QuitGame();
}


ASGameModeBase* USMainMenuWidget::GetGameMode()
{
    return GetWorld() ? Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

void USMainMenuWidget::OnGameStateChanged(ESGameState GameState)
{
    if (GameState == ESGameState::InProgress)
    {
        ChangeVisibilityOfChildWidgets(ESlateVisibility::Visible, ESlateVisibility::Hidden);
    }
    else
    {
        ChangeVisibilityOfChildWidgets(ESlateVisibility::Hidden, ESlateVisibility::Visible);

    }
}

void USMainMenuWidget::ChangeVisibilityOfChildWidgets(ESlateVisibility OrderGameWidgetVisibility,
    ESlateVisibility AllGamesResultsWidgetVisibility)
{
    OrderGameWidget->SetVisibility(OrderGameWidgetVisibility);
    AllGamesResultsWidget->SetVisibility(AllGamesResultsWidgetVisibility);
}
