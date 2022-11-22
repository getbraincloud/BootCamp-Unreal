// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "BrainCloudDialogWidget.h"
#include "DialogManager.h"
#include "NetworkManager.h"
#include "SpaceShooterGameModeBase.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"


bool UBrainCloudDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    AttachEmailButton->OnClicked.AddUniqueDynamic(this, &UBrainCloudDialogWidget::OnAttachEmailButtonClicked);
    ChangeUsernameButton->OnClicked.AddUniqueDynamic(this, &UBrainCloudDialogWidget::OnChangeUsernameButtonClicked);
    LeaderboardsButton->OnClicked.AddUniqueDynamic(this, &UBrainCloudDialogWidget::OnLeaderboardsButtonClicked);
    StatisticsButton->OnClicked.AddUniqueDynamic(this, &UBrainCloudDialogWidget::OnStatisticsButtonClicked);
    AchievementsButton->OnClicked.AddUniqueDynamic(this, &UBrainCloudDialogWidget::OnAchievementsButtonClicked);
    LogOutButton->OnClicked.AddUniqueDynamic(this, &UBrainCloudDialogWidget::OnLogOutButtonClicked);
    LogInButton->OnClicked.AddUniqueDynamic(this, &UBrainCloudDialogWidget::OnLogInButtonClicked);
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);
    
    return true;
}

void UBrainCloudDialogWidget::OnShow()
{
    Refresh();
}

UDialogWidget::Type UBrainCloudDialogWidget::GetType()
{
    return UDialogWidget::Type::BrainCloud;
}

void UBrainCloudDialogWidget::Refresh()
{
    if (GetNetworkManager()->IsAuthenticated())
    {
        LoggedInBox->SetVisibility(ESlateVisibility::Visible);
        LoggedOutBox->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        LoggedInBox->SetVisibility(ESlateVisibility::Hidden);
        LoggedOutBox->SetVisibility(ESlateVisibility::Visible);
    }
}

void UBrainCloudDialogWidget::OnAttachEmailButtonClicked()
{
    GetDialogManager()->ShowAttachEmailDialog();
}

void UBrainCloudDialogWidget::OnChangeUsernameButtonClicked()
{
    GetDialogManager()->ShowChangeUsernameDialog();
}

void UBrainCloudDialogWidget::OnLeaderboardsButtonClicked()
{
    GetDialogManager()->ShowLeaderboardsDialog();
}

void UBrainCloudDialogWidget::OnStatisticsButtonClicked()
{
    GetDialogManager()->ShowStatisticsDialog();
}

void UBrainCloudDialogWidget::OnAchievementsButtonClicked()
{
    GetDialogManager()->ShowAchievementsDialog();
}

void UBrainCloudDialogWidget::OnLogOutButtonClicked()
{
    GetNetworkManager()->LogOut();
}

void UBrainCloudDialogWidget::OnLogInButtonClicked()
{
    Hide();
    
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    if(gameMode != nullptr)
        gameMode->HandleAuthentication();
}
