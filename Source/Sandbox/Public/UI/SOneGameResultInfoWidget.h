// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "SOneGameResultInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class SANDBOX_API USOneGameResultInfoWidget : public UUserWidget
{
    GENERATED_BODY()

    UPROPERTY(meta=(BindWidget))
    UTextBlock* EndTime;
    
    UPROPERTY(meta=(BindWidget))
    UTextBlock* TotalTime;

public:
    void SetInfo(const FSGameResultInfo& GameInfo);
};
