// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SStoredInOrder.h"
#include "SCoreTypes.generated.h"

class ISStoredInOrder;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, ESGameState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrderOfObjsChangedSignature, const TArray<TScriptInterface<ISStoredInOrder>>, ArrayOfItemsInRightOrder);

UENUM(BlueprintType)
enum class ESGameState : uint8
{
    WaitingToStart,
    InProgress,
    Pause
};

UENUM(BlueprintType)
enum class ESPointerSourceSide : uint8
{
    NONE,
    Left,
    Right
};
