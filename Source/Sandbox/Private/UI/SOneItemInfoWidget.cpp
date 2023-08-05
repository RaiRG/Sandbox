// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SOneItemInfoWidget.h"

#include "Components/TextBlock.h"

void USOneItemInfoWidget::SetText(FText Text)
{
    TextBlock->SetText(Text);
}
