// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SMainMenuWidget.h"

#include "SGameModeBase.h"
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
}

void USMainMenuWidget::OnStartGame()
{
    if (!GetGameMode())
        return;
    UE_LOG(LogSMainMenuWidget, Display, TEXT("OnStartGame"));
    GetGameMode()->StartGame();
}

void USMainMenuWidget::OnQuitGame()
{
    if (!GetGameMode())
        return;
    //TODO: clear comments
    //GetGameMode()->QuitGame();
}


ASGameModeBase* USMainMenuWidget::GetGameMode()
{
    return GetWorld() ? Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}
