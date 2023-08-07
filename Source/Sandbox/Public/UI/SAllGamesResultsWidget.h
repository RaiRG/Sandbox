// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SCoreTypes.h"
#include "SAllGamesResultsWidget.generated.h"

class UVerticalBox;
class USOneGameResultInfoWidget;
/**
 * 
 */
UCLASS()
class SANDBOX_API USAllGamesResultsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;
    
    UPROPERTY(meta=(BindWidget))
    UVerticalBox* AllGamesListVerticalBox;
    
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<USOneGameResultInfoWidget> OneGameResultInfoWidgetClass;

    UPROPERTY(EditDefaultsOnly)
    int32 MaxNumberOfShowedResults = 3;
private:
    void FillWithInfoAboutPreviousGames();
    void OnGameStateChanged(ESGameState GameState);
};
