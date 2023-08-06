// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SOneGameResultInfoWidget.h"

#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(LogSOneGameResultInfoWidget, All, All);

void USOneGameResultInfoWidget::SetInfo(const FSGameResultInfo& GameInfo)
{

    auto Date = FText::FromString(GameInfo.TimeAtTheEnd.ToString());
    auto TotalTimeText = FText::FromString(FString::Printf(TEXT("%i:%i:%i"), GameInfo.TotalTime.GetHours(), GameInfo.TotalTime.GetMinutes(),
        GameInfo.TotalTime.GetSeconds()));

    if (EndTime)
        EndTime->SetText(Date);

    if (TotalTime)
        TotalTime->SetText(TotalTimeText);
}
