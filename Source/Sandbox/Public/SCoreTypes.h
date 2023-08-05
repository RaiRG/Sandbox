// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCoreTypes.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, ESTGameState);

UENUM(BlueprintType)
enum class ESTGameState : uint8
{
    WaitingToStart,
    InProgress,
    Pause
};