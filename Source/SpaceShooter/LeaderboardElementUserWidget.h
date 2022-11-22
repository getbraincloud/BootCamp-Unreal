// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LeaderboardEntry.h"
#include "Blueprint/UserWidget.h"
#include "LeaderboardElementUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API ULeaderboardElementUserWidget : public UUserWidget
{
	GENERATED_BODY()
    
public:
    void Reset();
    void SetLeaderboarEntry(LeaderboardEntry* leaderboardEntry);
    
private:
    UPROPERTY(meta = (BindWidget)) class UTextBlock* RankText;
    UPROPERTY(meta = (BindWidget)) class UTextBlock* NameText;
    UPROPERTY(meta = (BindWidget)) class UTextBlock* TimeText;
};
