// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "Constants.h"
#include "LeaderboardsDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API ULeaderboardsDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
	
protected:
    bool Initialize() override;
    void OnShow() override;

public:
    UDialogWidget::Type GetType() override;
    
private:
    enum class SegmentControlState
    {
        Main = 0,
        Daily
    };
    
    void SetSegmentControlState(SegmentControlState segmentControlState);
    void SetLeaderboardData(const FString& leaderboardId);
    void ResetLeaderboardData();
    FString GetTexturePath(SegmentControlState segmentControlState);
    bool SetButtonTextures(class UButton* button, const FString& path, bool isSelected);
    

    UFUNCTION(Category = "CallBack") void OnLeftSegmentButtonClicked();
    UFUNCTION(Category = "CallBack") void OnRightSegmentButtonClicked();

    
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
    
    UPROPERTY(meta = (BindWidgetOptional)) class UButton* SegmentControlLeft;
    UPROPERTY(meta = (BindWidgetOptional)) class UButton* SegmentControlRight;
    
    UPROPERTY(meta = (BindWidget)) class ULeaderboardElementUserWidget* LeaderboardElement_0;
    UPROPERTY(meta = (BindWidget)) class ULeaderboardElementUserWidget* LeaderboardElement_1;
    UPROPERTY(meta = (BindWidget)) class ULeaderboardElementUserWidget* LeaderboardElement_2;
    UPROPERTY(meta = (BindWidget)) class ULeaderboardElementUserWidget* LeaderboardElement_3;
    UPROPERTY(meta = (BindWidget)) class ULeaderboardElementUserWidget* LeaderboardElement_4;
    UPROPERTY(meta = (BindWidget)) class ULeaderboardElementUserWidget* LeaderboardElement_5;
    UPROPERTY(meta = (BindWidget)) class ULeaderboardElementUserWidget* LeaderboardElement_6;
    UPROPERTY(meta = (BindWidget)) class ULeaderboardElementUserWidget* LeaderboardElement_7;
    UPROPERTY(meta = (BindWidget)) class ULeaderboardElementUserWidget* LeaderboardElement_8;
    UPROPERTY(meta = (BindWidget)) class ULeaderboardElementUserWidget* LeaderboardElement_9;

    class ULeaderboardElementUserWidget* m_LeaderboardEntryElements[kMaxLeaderboardElements];
};
