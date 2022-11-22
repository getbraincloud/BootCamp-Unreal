// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "DialogManager.h"
#include "SpaceShooterGameModeBase.h"


UDialogManager::UDialogManager() :
    m_ConnectingDialog(nullptr),
    m_EmailLoginDialog(nullptr),
    m_UniversalLoginDialog(nullptr),
    m_MainMenuDialog(nullptr),
    m_BrainCloudDialog(nullptr),
    m_InGameBrainCloudDialog(nullptr),
    m_LevelSelectDialog(nullptr),
    m_PauseDialog(nullptr),
    m_AchievementsDialog(nullptr),
    m_StatisticsDialog(nullptr),
    m_LeaderboardsDialog(nullptr),
    m_ChangeUsernameDialog(nullptr),
    m_AttachEmailDialog(nullptr),
    m_PlayAgainDialog(nullptr),
    m_PostScoreDialog(nullptr),
    m_ErrorDialog(nullptr)
{

}

void UDialogManager::CreateDialogs()
{
    m_ConnectingDialog = CreateWidgetAndValidate<UConnectingDialogWidget>("ConnectingWidget");

//    m_EmailLoginDialog = CreateWidgetAndValidate<UEmailLoginDialogWidget>("EmailLoginWidget");
//    m_EmailLoginDialog = CreateWidgetAndValidate<UEmailLoginDialogWidget>("EmailAnonymousLoginWidget");
    m_EmailLoginDialog = CreateWidgetAndValidate<UEmailLoginDialogWidget>("EmailTwitchLoginWidget");

    m_UniversalLoginDialog = CreateWidgetAndValidate<UUniversalLoginDialogWidget>("UniversalLoginWidget");
    m_MainMenuDialog = CreateWidgetAndValidate<UMainMenuDialogWidget>("MainMenuWidget");
    m_BrainCloudDialog = CreateWidgetAndValidate<UBrainCloudDialogWidget>("BrainCloudWidget");
    m_InGameBrainCloudDialog = CreateWidgetAndValidate<UInGameBrainCloudDialogWidget>("InGameBrainCloudWidget");
    m_LevelSelectDialog = CreateWidgetAndValidate<ULevelSelectDialogWidget>("LevelSelectWidget");
    m_PauseDialog = CreateWidgetAndValidate<UPauseDialogWidget>("PauseWidget");
    m_AchievementsDialog = CreateWidgetAndValidate<UAchievementsDialogWidget>("AchievementsWidget");
    m_StatisticsDialog = CreateWidgetAndValidate<UStatisticsDialogWidget>("StatisticsWidget");
    m_LeaderboardsDialog = CreateWidgetAndValidate<ULeaderboardsDialogWidget>("LeaderboardsWidget");
    m_ChangeUsernameDialog = CreateWidgetAndValidate<UChangeUsernameDialogWidget>("ChangeUsernameWidget");
    m_AttachEmailDialog = CreateWidgetAndValidate<UAttachEmailDialogWidget>("AttachEmailWidget");
    m_PlayAgainDialog = CreateWidgetAndValidate<UPlayAgainDialogWidget>("PlayAgainWidget");
    m_PostScoreDialog = CreateWidgetAndValidate<UPostScoreDialogWidget>("PostScoreWidget");
    m_ErrorDialog = CreateWidgetAndValidate<UErrorDialogWidget>("ErrorWidget");
}

UDialogWidget* UDialogManager::GetDialogByType(UDialogWidget::Type type)
{
    if(type == UDialogWidget::Type::Connecting)
        return m_ConnectingDialog;
    else if(type == UDialogWidget::Type::EmailLogin)
        return m_EmailLoginDialog;
    else if(type == UDialogWidget::Type::UniversalLogin)
        return m_UniversalLoginDialog;
    else if(type == UDialogWidget::Type::MainMenu)
        return m_MainMenuDialog;
    else if(type == UDialogWidget::Type::BrainCloud)
        return m_BrainCloudDialog;
    else if(type == UDialogWidget::Type::InGameBrainCloud)
        return m_InGameBrainCloudDialog;
    else if(type == UDialogWidget::Type::LevelSelect)
        return m_LevelSelectDialog;
    else if(type == UDialogWidget::Type::Pause)
        return m_PauseDialog;
    else if(type == UDialogWidget::Type::Achievements)
        return m_AchievementsDialog;
    else if(type == UDialogWidget::Type::Statistics)
        return m_StatisticsDialog;
    else if(type == UDialogWidget::Type::Leaderboards)
        return m_LeaderboardsDialog;
    else if(type == UDialogWidget::Type::ChangeUsername)
        return m_ChangeUsernameDialog;
    else if(type == UDialogWidget::Type::AttachEmail)
        return m_AttachEmailDialog;
    else if(type == UDialogWidget::Type::PlayAgain)
        return m_PlayAgainDialog;
    else if(type == UDialogWidget::Type::Error)
        return m_ErrorDialog;
    return nullptr;
}

void UDialogManager::ShowConnectingDialog()
{
    if (!m_ConnectingDialog->IsShowing())
        Show(m_ConnectingDialog);
}

void UDialogManager::HideConnectingDialog()
{
    if (m_ConnectingDialog->IsShowing())
        m_ConnectingDialog->Hide();
}

void UDialogManager::ShowEmailLoginDialog()
{
    if (!m_EmailLoginDialog->IsShowing())
        Show(m_EmailLoginDialog);
}

void UDialogManager::ShowUniversalLoginDialog()
{
    if (!m_UniversalLoginDialog->IsShowing())
        Show(m_UniversalLoginDialog);
}

void UDialogManager::ShowMainMenuDialog()
{
    if (!m_MainMenuDialog->IsShowing())
        Show(m_MainMenuDialog);
}

void UDialogManager::ShowPauseDialog()
{
    if (!m_PauseDialog->IsShowing())
        Show(m_PauseDialog);
}

void UDialogManager::ShowBrainCloudDialog(bool inGameDialog)
{
    if (inGameDialog)
    {
        if (!m_InGameBrainCloudDialog->IsShowing())
            Show(m_InGameBrainCloudDialog);
    }
    else
    {
        if (!m_BrainCloudDialog->IsShowing())
            Show(m_BrainCloudDialog);
    }
}

void UDialogManager::ShowLevelSelectDialog()
{
    if (!m_LevelSelectDialog->IsShowing())
        Show(m_LevelSelectDialog);
}

void UDialogManager::ShowAchievementsDialog()
{
    if (!m_AchievementsDialog->IsShowing())
        Show(m_AchievementsDialog);
}

void UDialogManager::ShowStatisticsDialog()
{
    if (!m_StatisticsDialog->IsShowing())
        Show(m_StatisticsDialog);
}

void UDialogManager::ShowLeaderboardsDialog()
{
    if (!m_LeaderboardsDialog->IsShowing())
        Show(m_LeaderboardsDialog);
}

void UDialogManager::ShowChangeUsernameDialog()
{
    if (!m_ChangeUsernameDialog->IsShowing())
        Show(m_ChangeUsernameDialog);
}

void UDialogManager::ShowAttachEmailDialog()
{
    if (!m_AttachEmailDialog->IsShowing())
        Show(m_AttachEmailDialog);
}

void UDialogManager::ShowPlayAgainDialog()
{
    if (!m_PlayAgainDialog->IsShowing())
        Show(m_PlayAgainDialog);
}

void UDialogManager::ShowPostScoreDialog(float time)
{
    if (!m_PostScoreDialog->IsShowing())
    {
        m_PostScoreDialog->Set(time);
        Show(m_PostScoreDialog);
    }
}

void UDialogManager::ShowErrorDialog(const FString& errorMessage)
{
    if (!m_ErrorDialog->IsShowing())
    {
        m_ErrorDialog->SetMessage(errorMessage);
        Show(m_ErrorDialog);
    }
}

bool UDialogManager::AreAnyDialogsShowing()
{
    return DialogStackCount() > 0;
}

int UDialogManager::DialogStackCount()
{
    return m_ActiveDialogs.size();
}

void UDialogManager::OnEscape()
{
    UE_LOG(LogTemp, Warning, TEXT("UDialogManager::OnEscape()"));

    
    if (!AreAnyDialogsShowing())
    {
        if (GetGameMode()->IsGameOver() || GetGameMode()->IsGameWon())
            ShowPlayAgainDialog();
        else
            ShowPauseDialog();
    }
    else
    {
        if(m_ActiveDialogs.top() != m_MainMenuDialog)
            m_ActiveDialogs.top()->Hide();
    }
}

void UDialogManager::Show(UDialogWidget* dialog)
{
    if (m_ActiveDialogs.empty())
        GetGameMode()->SetTimeScale(0.0f);
    else
        m_ActiveDialogs.top()->Hide(false);

    m_ActiveDialogs.push(dialog);

    dialog->Show();
}

void UDialogManager::OnDialogShown(UDialogWidget* dialog)
{
    
}

void UDialogManager::OnDialogHidden(UDialogWidget* dialog)
{
    UE_LOG(LogTemp, Warning, TEXT("UDialogManager::OnDialogHidden(%p, %u)"), dialog, m_ActiveDialogs.size());


    m_ActiveDialogs.pop();

    if (m_ActiveDialogs.size() == 0)
    {
        GetGameMode()->SetTimeScale(1.0f);
    }
    else
        m_ActiveDialogs.top()->Show(false);
}

ASpaceShooterGameModeBase* UDialogManager::GetGameMode()
{
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    return gameMode;
}
