// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SDynamicListOfItemsWidget.generated.h"

class ISStoredInOrder;
class UVerticalBox;
class USOneItemInfoWidget;
class ASGameModeBase;
class ISStoreObjsInOrder;
/**
 * 
 */
UCLASS()
class SANDBOX_API USDynamicListOfItemsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta=(BindWidget))
    UVerticalBox* VerticalBox;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<USOneItemInfoWidget> OneItemInfoWidgetClass;
    
    UPROPERTY(EditDefaultsOnly)
    int32 MaxNumberOfShowedItem = 3;
private:
    TScriptInterface<ISStoreObjsInOrder> ActorStoresObjInOrder;
    UPROPERTY()
    TArray<USOneItemInfoWidget*> WidgetsWithOrderedItemsInfo;
    
    UFUNCTION()
    void ChangeOrder(TArray<TScriptInterface<ISStoredInOrder>> Order);

    UFUNCTION()
    void OnGameStateChanged(ESGameState State);
    ASGameModeBase* GetGameMode();
};
