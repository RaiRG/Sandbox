// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SNeedOrderListWidget.h"

#include "SGameModeBase.h"
#include "SOneItemInfoWidget.h"
#include "Components/VerticalBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogSNeedOrderListWidget, All, All);


void USNeedOrderListWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    UE_LOG(LogSNeedOrderListWidget, Display, TEXT("NativeOnInitialized"));

    if (auto GameMode = GetGameMode())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &USNeedOrderListWidget::OnGameStateChanged);
    }

}

ASGameModeBase* USNeedOrderListWidget::GetGameMode()
{
    return Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode());
}

void USNeedOrderListWidget::OnGameStateChanged(ESGameState State)
{
    if (State != ESGameState::InProgress)
        return;
    if (const auto GameMode = GetGameMode())
    {
        VerticalBox->ClearChildren();
        auto NeedOrder = GameMode->GetNeededOrderOfItems();
        for (auto IsStoredInOrder : NeedOrder)
        {
            if (auto Widget = Cast<USOneItemInfoWidget>(CreateWidget(this, OneItemInfoWidgetClass)))
            {
                VerticalBox->AddChild(Widget);
                if (IsStoredInOrder)
                {
                    Widget->SetIcon(IsStoredInOrder->GetIcon());
                }
            }
        }
    }
}
