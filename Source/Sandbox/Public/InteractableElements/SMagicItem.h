// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickableUp.h"
#include "SStoredInOrder.h"
#include "GameFramework/Actor.h"
#include "SMagicItem.generated.h"

class USMagicItemProperties;
class UNiagaraComponent;
UCLASS()
class SANDBOX_API ASMagicItem : public AActor, public ISPickableUp, public ISStoredInOrder
{
    GENERATED_BODY()

public:
    ASMagicItem();

    virtual void PickUp(TScriptInterface<ISObjectsHolder> Holder, UMeshComponent* MeshForAttaching) override;
    virtual void Drop() override;
    virtual FVector GetLocationInWorld() override;
    virtual bool CanBePlaced() override {return bCanBePlacedOnTable; }
    virtual UTexture2D* GetIcon() override;
protected:
    virtual void BeginPlay() override;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    USceneComponent* SceneComponent;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UNiagaraComponent* MagicTrackComponent;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
    USMagicItemProperties* Properties;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Properties")
    FName VariableNameOfColor = "User.ColorOfTrack";
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PickableUp")
    FName SocketForAttaching = "SocketForAttaching";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PickableUp")
    FName TestName;
private:
    bool bCanBePlacedOnTable = true;

    //PickUp:
    bool bIsPickedUp = false;
    TScriptInterface<ISObjectsHolder> HolderObj;
    
    FTransform StartStateRelativeTransformOfStaticMesh;
 
};
