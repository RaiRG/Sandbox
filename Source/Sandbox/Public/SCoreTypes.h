// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCoreTypes.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, ESGameState);

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
