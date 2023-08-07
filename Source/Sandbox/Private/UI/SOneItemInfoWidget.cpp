// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SOneItemInfoWidget.h"
#include "Components/Image.h"

void USOneItemInfoWidget::SetIcon(UTexture2D* NewIcon)
{
    Icon->SetBrushFromTexture(NewIcon);
}
