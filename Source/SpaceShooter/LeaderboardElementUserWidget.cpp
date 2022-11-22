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

void ULeaderboardElementUserWidget::SetLeaderboarEntry(LeaderboardEntry* leaderboardEntry)
{
    FString rank = FString::FromInt(leaderboardEntry->GetRank()) + ".";
    RankText->SetText(FText::FromString(rank));
    
    FString name = leaderboardEntry->GetNickname();
    NameText->SetText(FText::FromString(name));
    
    float time = leaderboardEntry->GetTime();
    FString timeStr = StringUtils::FormatTime(time);
    TimeText->SetText(FText::FromString(timeStr));
    
    if(leaderboardEntry->IsUserScore())
    {
        FLinearColor color(FColor(207, 198, 0, 255));
        RankText->SetColorAndOpacity(FSlateColor(color));
        NameText->SetColorAndOpacity(FSlateColor(color));
        TimeText->SetColorAndOpacity(FSlateColor(color));
    }
    else
    {
        FLinearColor color(FColor(255, 255, 255, 255));
        RankText->SetColorAndOpacity(FSlateColor(color));
        NameText->SetColorAndOpacity(FSlateColor(color));
        TimeText->SetColorAndOpacity(FSlateColor(color));
    }
}
