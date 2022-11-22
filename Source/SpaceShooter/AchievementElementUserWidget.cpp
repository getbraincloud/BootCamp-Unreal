// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "AchievementElementUserWidget.h"
#include "Components/TextBlock.h"



void UAchievementElementUserWidget::Reset()
{
    DescriptionText->SetText(FText::FromString(TEXT("")));
    StatusText->SetText(FText::FromString(TEXT("")));
}

void UAchievementElementUserWidget::SetAchievement(Achievement* achievement)
{
    FString description = achievement->GetDescription();
    DescriptionText->SetText(FText::FromString(description));
    
    FString status = achievement->GetStatusString();
    StatusText->SetText(FText::FromString(status));
    
    if (achievement->IsAwarded())
    {
        FLinearColor color(FColor(207, 198, 0, 255));
        DescriptionText->SetColorAndOpacity(FSlateColor(color));
        StatusText->SetColorAndOpacity(FSlateColor(color));
    }
    else
    {
        FLinearColor color(FColor(255, 255, 255, 255));
        DescriptionText->SetColorAndOpacity(FSlateColor(color));
        StatusText->SetColorAndOpacity(FSlateColor(color));
    }
}
    
