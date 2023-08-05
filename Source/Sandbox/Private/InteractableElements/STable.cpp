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
        if (ISStoredInOrder* PlacedOnTableActor = Cast<ISStoredInOrder>(OtherActor))
        {
            const auto WorldLocation = PlacedOnTableActor->GetLocationInWorld();
            const FVector ProjectedPoint = FVector::DotProduct(WorldLocation, DirectionOfOrder->GetForwardVector()) * DirectionOfOrder->
                                           GetForwardVector();
            ActorsPlacedOnTable.Add(PlacedOnTableActor);
            ActorsMapping.Add(OtherActor);
            ProjectedPoints.Add(ProjectedPoint);
            ProjectedPoints.Sort([&](const FVector& A, const FVector& B)
            {
                return FVector::DotProduct(A, DirectionOfOrder->GetForwardVector()) < FVector::DotProduct(B,
                           DirectionOfOrder->GetForwardVector());
            });
            ItemProjectedPointMapping.Add(PlacedOnTableActor, ProjectedPoint);
            ProjectedPointToItemMapping.Add(ProjectedPoint, PlacedOnTableActor);

            UE_LOG(LogSTable, Display, TEXT("Order:"));

            for (FVector Point : ProjectedPoints)
            {
                int32 Index;
                ActorsPlacedOnTable.Find(ProjectedPointToItemMapping[Point], Index);

                auto Ptr = TScriptInterface<ISStoredInOrder>();
                Ptr.SetInterface(ProjectedPointToItemMapping[Point]);
                Ptr.SetObject(ActorsMapping[Index]);
                ObjsInRightOrder.Add(Ptr);
                if (auto Int = Ptr.GetInterface())
                {
                    UE_LOG(LogSTable, Display, TEXT("%s"), *Int->GetNameForOrder().ToString());
                }
                else
                {
                    UE_LOG(LogSTable, Warning, TEXT("Int is null"));

                }
            }

            OnOrderOfObjsChanged.Broadcast(ObjsInRightOrder);
            ObjsInRightOrder.Empty();
        }
    }
}

void ASTable::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);
    {
        if (ISStoredInOrder* PlacedOnTable = Cast<ISStoredInOrder>(OtherActor))
        {
            ActorsPlacedOnTable.Remove(PlacedOnTable);
            if (ItemProjectedPointMapping.Contains(PlacedOnTable))
            {
                const FVector ProjectedPoint = ItemProjectedPointMapping[PlacedOnTable];
                ItemProjectedPointMapping.Remove(PlacedOnTable);
                ProjectedPointToItemMapping.Remove(ProjectedPoint);
                ProjectedPoints.Remove(ProjectedPoint);
            }
        }
    }
}
