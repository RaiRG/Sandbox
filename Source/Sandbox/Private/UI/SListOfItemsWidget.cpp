// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SListOfItemsWidget.h"

#include "SOneItemInfoWidget.h"
#include "SStoredInOrder.h"
#include "SStoreObjsInOrder.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSListOfItemsWidget, All, All);

void USListOfItemsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    for (int i = 0; i < MaxNumberOfShowedItem; i++)
    {
        auto Widget = Cast<USOneItemInfoWidget>(CreateWidget(this, OneItemInfoWidgetClass));
        if (Widget)
        {
            VerticalBox->AddChild(Widget);
            ItemsInOrder.Add(Widget);
            Widget->SetText(FText());
        }
    }

    TArray<AActor*> OutActors;
    UGameplayStatics::GetAllActorsWithInterface(this, USStoreObjsInOrder::StaticClass(), OutActors);
    for (auto OutActor : OutActors)
    {
        if (UKismetSystemLibrary::DoesImplementInterface(OutActor, USStoreObjsInOrder::StaticClass()))
        {
            if (ISStoreObjsInOrder* ActorStoredInOrder = Cast<ISStoreObjsInOrder>(OutActor))
            {
                ActorStoredInOrder->GetOnOrderOfObjsChangedSignature().AddDynamic(this, &USListOfItemsWidget::ChangeOrder);
            }
        }
    }
}

void USListOfItemsWidget::ChangeOrder(TArray<TScriptInterface<ISStoredInOrder>> Order)
{
    int32 Index = 0;
    for (auto OrderWidget : ItemsInOrder)
    {
        if (Index < Order.Num())
        {
            auto Item = Order[Index].GetInterface();
            if (Item)
            {
                auto Str = Item->GetNameForOrder().ToString();
                UE_LOG(LogSListOfItemsWidget, Display, TEXT("Val %s"), *Str);
                const FText Text = FText::FromString(Str);
                OrderWidget->SetText(Text);
                Index++;
            }
        }
        else
        {
            break;
        }
    }
}
