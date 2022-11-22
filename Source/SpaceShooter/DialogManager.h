// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "EmailLoginDialogWidget.h"
#include "UniversalLoginDialogWidget.h"
#include "MainMenuDialogWidget.h"
#include "ConnectingDialogWidget.h"
#include "BrainCloudDialogWidget.h"
#include "InGameBrainCloudDialogWidget.h"
#include "LevelSelectDialogWidget.h"
#include "PauseDialogWidget.h"
#include "AchievementsDialogWidget.h"
#include "StatisticsDialogWidget.h"
#include "LeaderboardsDialogWidget.h"
#include "ChangeUsernameDialogWidget.h"
#include "AttachEmailDialogWidget.h"
#include "PlayAgainDialogWidget.h"
#include "PostScoreDialogWidget.h"
#include "ErrorDialogWidget.h"
#include "Constants.h"
#include <stack>
#include "UObject/ConstructorHelpers.h"
#include "UObject/NoExportTypes.h"
#include "DialogManager.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UDialogManager : public UObject, public DialogWidgetCallback
{
	GENERATED_BODY()
    
public:
    UDialogManager();
    ~UDialogManager() = default;
    
    void CreateDialogs();
    
    UDialogWidget* GetDialogByType(UDialogWidget::Type type);
    
    void ShowConnectingDialog();
    void HideConnectingDialog();

    void ShowEmailLoginDialog();
    void ShowUniversalLoginDialog();
    void ShowMainMenuDialog();
    void ShowPauseDialog();
    void ShowBrainCloudDialog(bool inGameDialog = false);
    void ShowLevelSelectDialog();
    void ShowAchievementsDialog();
    void ShowStatisticsDialog();
    void ShowLeaderboardsDialog();
    void ShowChangeUsernameDialog();
    void ShowAttachEmailDialog();
    void ShowPlayAgainDialog();
    void ShowPostScoreDialog(float time);
    void ShowErrorDialog(const FString& errorMessage);
    
    bool AreAnyDialogsShowing();
    int DialogStackCount();

    void OnEscape();
    
private:
    template<typename T> T* CreateWidgetAndValidate(const FString& name)
    {
        FString directory = "/Game/Widgets/";
        FString path = directory + name + "." + name + "_C";
        UClass* widgetClass = StaticLoadClass(T::StaticClass(), nullptr, *path);
        check(widgetClass != nullptr);
        
        UUserWidget* userWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
        check(userWidget != nullptr);
        
        userWidget->SetDesiredSizeInViewport(FVector2D(kScreenWidth, kScreenHeight));
        userWidget->SetAnchorsInViewport(FAnchors(0.5f, 0.5f));
        userWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
        
        T* castedWidget = Cast<T>(userWidget);
        check(castedWidget != nullptr);

        castedWidget->SetCallback(this);

        return castedWidget;
    }
    
    void Show(UDialogWidget* dialog);
    
    void OnDialogShown(UDialogWidget* dialog) override;
    void OnDialogHidden(UDialogWidget* dialog) override;
    
    class ASpaceShooterGameModeBase* GetGameMode();
    
    std::stack<UDialogWidget*> m_ActiveDialogs;
    
    UPROPERTY() UConnectingDialogWidget* m_ConnectingDialog;
    UPROPERTY() UEmailLoginDialogWidget* m_EmailLoginDialog;
    UPROPERTY() UUniversalLoginDialogWidget* m_UniversalLoginDialog;
    UPROPERTY() UMainMenuDialogWidget* m_MainMenuDialog;
    UPROPERTY() UBrainCloudDialogWidget* m_BrainCloudDialog;
    UPROPERTY() UInGameBrainCloudDialogWidget* m_InGameBrainCloudDialog;
    UPROPERTY() ULevelSelectDialogWidget* m_LevelSelectDialog;
    UPROPERTY() UPauseDialogWidget* m_PauseDialog;
    UPROPERTY() UAchievementsDialogWidget* m_AchievementsDialog;
    UPROPERTY() UStatisticsDialogWidget* m_StatisticsDialog;
    UPROPERTY() ULeaderboardsDialogWidget* m_LeaderboardsDialog;
    UPROPERTY() UChangeUsernameDialogWidget* m_ChangeUsernameDialog;
    UPROPERTY() UAttachEmailDialogWidget* m_AttachEmailDialog;
    UPROPERTY() UPlayAgainDialogWidget* m_PlayAgainDialog;
    UPROPERTY() UPostScoreDialogWidget* m_PostScoreDialog;
    UPROPERTY() UErrorDialogWidget* m_ErrorDialog;
};
