// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SMainMenuWidget.generated.h"

class ASGameModeBase;
class UButton;

UCLASS()
class SANDBOX_API USMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

    UPROPERTY(meta=(BindWidget))
    UButton* StartButton;

    UPROPERTY(meta=(BindWidget))
    UButton* ShowResultButton;

    UPROPERTY(meta=(BindWidget))
    UButton* QuitButton;

protected:
    virtual void NativeOnInitialized() override;
    
    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();
private:
    ASGameModeBase* GetGameMode();
    
};
