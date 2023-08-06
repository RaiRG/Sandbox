// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SStoredInOrder.generated.h"

class ASTable;
// This class does not need to be modified.
UINTERFACE()
class USStoredInOrder : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class SANDBOX_API ISStoredInOrder
{
    GENERATED_BODY()

public:
    UFUNCTION()
    virtual FVector GetLocationInWorld() = 0;
    UFUNCTION()
    virtual FName GetNameForOrder() = 0;
    UFUNCTION()
    virtual bool CanBePlaced() = 0;
};
