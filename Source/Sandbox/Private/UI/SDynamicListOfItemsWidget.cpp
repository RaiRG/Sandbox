// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SDynamicListOfItemsWidget.h"

#include "SGameModeBase.h"
#include "SOneItemInfoWidget.h"
#include "SStoredInOrder.h"
#include "SStoreObjsInOrder.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSListOfItemsWidget, All, All);

void USDynamicListOfItemsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    for (int i = 0; i < MaxNumberOfShowedItem; i++)
    {
        if (auto Widget = Cast<USOneItemInfoWidget>(CreateWidget(this, OneItemInfoWidgetClass)))
        {
            VerticalBox->AddChild(Widget);
            WidgetsWithOrderedItemsInfo.Add(Widget);
            //Widget->(FText());
        }
    }

    if (auto GameMode = GetGameMode())
    {
        GameMode->OnGameStateChanged.AddUObject(this, &USDynamicListOfItemsWidget::OnGameStateChanged);
    }
}

void USDynamicListOfItemsWidget::ChangeOrder(TArray<TScriptInterface<ISStoredInOrder>> Order)
{
    UE_LOG(LogSListOfItemsWidget, Display, TEXT("ChangeOrder"));
    int32 Index = 0;
    for (auto OrderWidget : WidgetsWithOrderedItemsInfo)
    {
        if (Index < Order.Num())
        {
            auto Item = Order[Index].GetInterface();
            if (Item && OrderWidget)
            {
                OrderWidget->SetIcon(Item->GetIcon());
                Index++;
            }
        }
        else
        {
            if (OrderWidget)
            {
                OrderWidget->SetIcon(nullptr);
            }
        }
    }
}

ASGameModeBase* USDynamicListOfItemsWidget::GetGameMode()
{
    return Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode());
}

void USDynamicListOfItemsWidget::OnGameStateChanged(ESGameState State)
{
    UE_LOG(LogSListOfItemsWidget, Display, TEXT("OnGameStateChanged"));

    if (State == ESGameState::End)
    {
        if (ActorStoresObjInOrder)
        {
            ActorStoresObjInOrder->GetOnOrderOfObjsChangedSignature().RemoveDynamic(this, &USDynamicListOfItemsWidget::ChangeOrder);
        }
    }
    if (State == ESGameState::InProgress)
    {
        TArray<AActor*> OutActors;
        UGameplayStatics::GetAllActorsWithInterface(this, USStoreObjsInOrder::StaticClass(), OutActors);
        for (auto OutActor : OutActors)
        {
            ActorStoresObjInOrder = TScriptInterface<ISStoreObjsInOrder>(OutActor);
            if (ActorStoresObjInOrder)
            {
                ActorStoresObjInOrder->GetOnOrderOfObjsChangedSignature().AddDynamic(this, &USDynamicListOfItemsWidget::ChangeOrder);
            }

        }
    }
}
