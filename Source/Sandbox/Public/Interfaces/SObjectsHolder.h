// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SObjectsHolder.generated.h"

class ISPickableUp;
UINTERFACE()
class USObjectsHolder : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class SANDBOX_API ISObjectsHolder
{
    GENERATED_BODY()

public:
    virtual bool GetDoesHold() =0;
    virtual void Hold(AActor* ObjectForHolding,  USceneComponent* HoldingComponent) = 0;
    virtual void ThrowOut() = 0;
};
