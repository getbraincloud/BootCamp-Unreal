// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "LeaderboardElementUserWidget.h"
#include "StringUtils.h"
#include "Components/TextBlock.h"


void ULeaderboardElementUserWidget::Reset()
{
    RankText->SetText(FText::FromString(TEXT("")));
    NameText->SetText(FText::FromString(TEXT("")));
    TimeText->SetText(FText::FromString(TEXT("")));
}
