// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "InGameBrainCloudDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UInGameBrainCloudDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;
    
public:
    UDialogWidget::Type GetType() override;
    
private:
    UFUNCTION(Category = "CallBack") void OnChangeUsernameButtonClicked();
    UFUNCTION(Category = "CallBack") void OnLeaderboardsButtonClicked();
    UFUNCTION(Category = "CallBack") void OnStatisticsButtonClicked();
    UFUNCTION(Category = "CallBack") void OnAchievementsButtonClicked();
    
    UPROPERTY(meta = (BindWidget)) class UButton* ChangeUsernameButton;
    UPROPERTY(meta = (BindWidget)) class UButton* LeaderboardsButton;
    UPROPERTY(meta = (BindWidget)) class UButton* StatisticsButton;
    UPROPERTY(meta = (BindWidget)) class UButton* AchievementsButton;
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
};
