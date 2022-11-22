// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "StatisticElementUserWidget.h"
#include "Components/TextBlock.h"

void UStatisticElementUserWidget::Reset()
{
    NameText->SetText(FText::FromString(TEXT("")));
    ValueText->SetText(FText::FromString(TEXT("")));
}

void UStatisticElementUserWidget::SetStatistic(Statistic* statistic)
{
    FString description = statistic->GetDescription();
    NameText->SetText(FText::FromString(description));
        
    FString status = FString::FromInt(statistic->GetValue());
    ValueText->SetText(FText::FromString(status));
}
