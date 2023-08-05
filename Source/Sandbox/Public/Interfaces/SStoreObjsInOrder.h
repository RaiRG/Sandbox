// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SCoreTypes.h"
#include "SStoreObjsInOrder.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USStoreObjsInOrder : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class SANDBOX_API ISStoreObjsInOrder
{
    GENERATED_BODY()

public:
    UFUNCTION()
    virtual FOnOrderOfObjsChangedSignature& GetOnOrderOfObjsChangedSignature() = 0;

};
