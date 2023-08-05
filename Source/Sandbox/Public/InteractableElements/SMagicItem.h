// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickableUp.h"
#include "SStoredInOrder.h"
#include "GameFramework/Actor.h"
#include "SMagicItem.generated.h"

class UNiagaraComponent;
UCLASS()
class SANDBOX_API ASMagicItem : public AActor, public ISPickableUp, public ISStoredInOrder
{
    GENERATED_BODY()

public:
    ASMagicItem();

    virtual void PickUp(UMeshComponent* MeshForAttaching) override;
    virtual void Drop() override;
    virtual FVector GetLocationInWorld() override;
    virtual FName  GetNameForOrder() override;
protected:
    virtual void BeginPlay() override;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UNiagaraComponent* MagicTrackComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PickableUp")
    USoundBase* PickUpSound;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PickableUp")
    FName SocketForAttaching = "SocketForAttaching";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PickableUp")
    FName TestName;
private:
    bool bIsPickedUp = false;
    FTransform StartStateRelativeTransformOfStaticMesh;
};
