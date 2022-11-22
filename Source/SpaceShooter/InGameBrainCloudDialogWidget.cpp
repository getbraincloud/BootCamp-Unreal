// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "InGameBrainCloudDialogWidget.h"
#include "DialogManager.h"
#include "Components/Button.h"


bool UInGameBrainCloudDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    ChangeUsernameButton->OnClicked.AddUniqueDynamic(this, &UInGameBrainCloudDialogWidget::OnChangeUsernameButtonClicked);
    LeaderboardsButton->OnClicked.AddUniqueDynamic(this, &UInGameBrainCloudDialogWidget::OnLeaderboardsButtonClicked);
    StatisticsButton->OnClicked.AddUniqueDynamic(this, &UInGameBrainCloudDialogWidget::OnStatisticsButtonClicked);
    AchievementsButton->OnClicked.AddUniqueDynamic(this, &UInGameBrainCloudDialogWidget::OnAchievementsButtonClicked);
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);
    
    return true;
}

UDialogWidget::Type UInGameBrainCloudDialogWidget::GetType()
{
    return UDialogWidget::Type::InGameBrainCloud;
}

void UInGameBrainCloudDialogWidget::OnChangeUsernameButtonClicked()
{
    GetDialogManager()->ShowChangeUsernameDialog();
}

void UInGameBrainCloudDialogWidget::OnLeaderboardsButtonClicked()
{
    GetDialogManager()->ShowLeaderboardsDialog();
}

void UInGameBrainCloudDialogWidget::OnStatisticsButtonClicked()
{
    GetDialogManager()->ShowStatisticsDialog();
}

void UInGameBrainCloudDialogWidget::OnAchievementsButtonClicked()
{
    GetDialogManager()->ShowAchievementsDialog();
}
