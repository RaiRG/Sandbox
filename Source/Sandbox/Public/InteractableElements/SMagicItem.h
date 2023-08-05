// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickableUp.h"
#include "Camera/CameraActor.h"
#include "GameFramework/Actor.h"
#include "SMagicItem.generated.h"

UCLASS()
class SANDBOX_API ASMagicItem : public AActor, public ISPickableUp
{
    GENERATED_BODY()

public:
    ASMagicItem();

    virtual void PickUp(UMeshComponent* MeshForAttaching) override;
    virtual void Drop() override;

protected:
    virtual void BeginPlay() override;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UStaticMeshComponent* StaticMeshComponent;


    UPROPERTY(EditDefaultsOnly)
    FName SocketForAttaching = "SocketForAttaching";

private:
    bool bIsPickedUp = false;
    FTransform StartStateRelativeTransformOfStaticMesh;
};
