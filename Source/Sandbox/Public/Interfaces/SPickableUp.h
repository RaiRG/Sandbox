// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPickableUp.generated.h"

class ISObjectsHolder;
UINTERFACE()
class USPickableUp : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class SANDBOX_API ISPickableUp
{
    GENERATED_BODY()

public:
    UFUNCTION()
    virtual void PickUp(TScriptInterface<ISObjectsHolder> Holder, UMeshComponent* MeshForAttaching) = 0;
    UFUNCTION()
    virtual void Drop() = 0;

};
