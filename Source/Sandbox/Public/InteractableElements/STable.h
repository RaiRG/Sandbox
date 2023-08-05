// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SStoreObjsInOrder.h"
#include "GameFramework/Actor.h"
#include "STable.generated.h"

class UArrowComponent;
class UBoxComponent;
class ISStoredInOrder;
UCLASS()
class SANDBOX_API ASTable : public AActor, public ISStoreObjsInOrder
{
    GENERATED_BODY()

public:
    ASTable();
    virtual FOnOrderOfObjsChangedSignature& GetOnOrderOfObjsChangedSignature() override {return OnOrderOfObjsChanged; };
protected:
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
    USceneComponent* SceneComponent;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
    UBoxComponent* BoxComponent;

    UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
    UArrowComponent* DirectionOfOrder;
    
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
    FOnOrderOfObjsChangedSignature OnOrderOfObjsChanged;
private:
    UPROPERTY()
    TArray<AActor*> ActorsMapping;
    TArray<ISStoredInOrder*> ActorsPlacedOnTable;
    TArray<FVector> ProjectedPoints;
    
    TMap<FVector, ISStoredInOrder*> ProjectedPointToItemMapping;
    TMap<ISStoredInOrder*, FVector> ItemProjectedPointMapping;
    
    TArray<TScriptInterface<ISStoredInOrder>> ObjsInRightOrder;
};
