// Fill out your copyright notice in the Description page of Project Settings.


#include "STable.h"
#include "UObject/ScriptInterface.h"
#include "SStoredInOrder.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTable, All, All);


ASTable::ASTable()
{
    PrimaryActorTick.bCanEverTick = true;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
    BoxComponent->SetupAttachment(GetRootComponent());

    DirectionOfOrder = CreateDefaultSubobject<UArrowComponent>("DirectionAgainstObserver");
    DirectionOfOrder->SetupAttachment(GetRootComponent());
}

void ASTable::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, USStoredInOrder::StaticClass()))
    {
        WatchFor(TScriptInterface<ISStoredInOrder>(OtherActor));
    }
}

void ASTable::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);
    {
        if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, USStoredInOrder::StaticClass()))
        {
            StopWatchingFor(TScriptInterface<ISStoredInOrder>(OtherActor));
        }
    }
}

void ASTable::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    //смотрим, какие объекты на столе доступны для составления последовательности, находим у них проекцию на нужные вектор и запоминаем необходимые данные
    TArray<TTuple<FVector, TScriptInterface<ISStoredInOrder>>> ProjectedPointsOfAvialableItems;
    for (int i = 0; i < AllWatchedItemsInt.Num(); i++)
    {
        auto WatchedItem = AllWatchedItemsInt[i];
        if (WatchedItem->CanBePlaced())
        {
            const auto WorldLocation = WatchedItem->GetLocationInWorld();
            const FVector ProjectedPoint = FVector::DotProduct(WorldLocation, DirectionOfOrder->GetForwardVector()) * DirectionOfOrder->
                                           GetForwardVector();
            ProjectedPointsOfAvialableItems.Add(TTuple<FVector, TScriptInterface<ISStoredInOrder>>(ProjectedPoint, WatchedItem));
        }
    }

    //сортируем все точки
    ProjectedPointsOfAvialableItems.Sort([&](const TTuple<FVector, TScriptInterface<ISStoredInOrder>>& A,
        const TTuple<FVector, TScriptInterface<ISStoredInOrder>>& B)
        {
            return FVector::DotProduct(A.Key, DirectionOfOrder->GetForwardVector()) < FVector::DotProduct(B.Key,
                       DirectionOfOrder->GetForwardVector());
        });

    TArray<TScriptInterface<ISStoredInOrder>> ObjectsInRightOrder;
    for (auto Point : ProjectedPointsOfAvialableItems)
    {
        auto ItemInfo = Point.Value;
        ObjectsInRightOrder.Add(ItemInfo);
    }
    OnOrderOfObjsChanged.Broadcast(ObjectsInRightOrder);
}

void ASTable::WatchFor(TScriptInterface<ISStoredInOrder> Item)
{
    AllWatchedItemsInt.Add(Item);
}

void ASTable::StopWatchingFor(TScriptInterface<ISStoredInOrder> ItemInterf)
{

    AllWatchedItemsInt.Remove(ItemInterf);

}
