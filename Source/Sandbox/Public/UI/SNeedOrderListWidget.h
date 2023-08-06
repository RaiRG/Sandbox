// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "SNeedOrderListWidget.generated.h"

class ASGameModeBase;
class UVerticalBox;
class USOneItemInfoWidget;
UCLASS()
class SANDBOX_API USNeedOrderListWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta=(BindWidget))
    UVerticalBox* VerticalBox;
    
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<USOneItemInfoWidget> OneItemInfoWidgetClass;
private:
    UFUNCTION()
    void OnGameStateChanged(ESGameState State);
    ASGameModeBase* GetGameMode();
};
