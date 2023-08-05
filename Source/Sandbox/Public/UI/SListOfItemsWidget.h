// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SListOfItemsWidget.generated.h"

class ISStoredInOrder;
class UVerticalBox;
class USOneItemInfoWidget;
/**
 * 
 */
UCLASS()
class SANDBOX_API USListOfItemsWidget : public UUserWidget
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
    UPROPERTY()
    TArray<USOneItemInfoWidget*> ItemsInOrder;
    UFUNCTION()
    void ChangeOrder(TArray<TScriptInterface<ISStoredInOrder>> Order);
};
