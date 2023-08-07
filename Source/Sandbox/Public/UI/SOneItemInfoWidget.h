// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SOneItemInfoWidget.generated.h"

class UImage;


UCLASS()
class SANDBOX_API USOneItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void SetIcon(UTexture2D* NewIcon);
protected:
    UPROPERTY(meta=(BindWidget))
    UImage* Icon;
};
