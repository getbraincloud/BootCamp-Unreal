// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "AchievementElementUserWidget.h"
#include "Components/TextBlock.h"



void UAchievementElementUserWidget::Reset()
{
    DescriptionText->SetText(FText::FromString(TEXT("")));
    StatusText->SetText(FText::FromString(TEXT("")));
}
