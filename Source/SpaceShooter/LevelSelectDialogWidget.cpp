// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "LevelSelectDialogWidget.h"
#include "DialogManager.h"
#include "SpaceShooterGameModeBase.h"
#include "UserData.h"
#include "Constants.h"
#include "Components/Button.h"


bool ULevelSelectDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;

    LevelOneButton->OnClicked.AddUniqueDynamic(this, &ULevelSelectDialogWidget::OnLevelOneButtonClicked);
    LevelTwoButton->OnClicked.AddUniqueDynamic(this, &ULevelSelectDialogWidget::OnLevelTwoButtonClicked);
    LevelThreeButton->OnClicked.AddUniqueDynamic(this, &ULevelSelectDialogWidget::OnLevelThreeButtonClicked);
    LevelBossButton->OnClicked.AddUniqueDynamic(this, &ULevelSelectDialogWidget::OnLevelBossButtonClicked);
    MainMenuButton->OnClicked.AddUniqueDynamic(this, &ULevelSelectDialogWidget::OnMainMenuButtonClicked);
    
    return true;
}

UDialogWidget::Type ULevelSelectDialogWidget::GetType()
{
    return UDialogWidget::Type::LevelSelect;
}

void ULevelSelectDialogWidget::OnShow()
{
    LevelTwoButton->SetIsEnabled(GetGameMode()->GetUserData()->HasLevelOneBeenCompleted());
    LevelThreeButton->SetIsEnabled(GetGameMode()->GetUserData()->HasLevelTwoBeenCompleted());
    LevelBossButton->SetIsEnabled(GetGameMode()->GetUserData()->HasLevelThreeBeenCompleted());
}

void ULevelSelectDialogWidget::OnLevelOneButtonClicked()
{
    Hide();
    GetGameMode()->StartHordeMode(kHordeModeLevelOne);
}

void ULevelSelectDialogWidget::OnLevelTwoButtonClicked()
{
    Hide();
    GetGameMode()->StartHordeMode(kHordeModeLevelTwo);
}

void ULevelSelectDialogWidget::OnLevelThreeButtonClicked()
{
    Hide();
    GetGameMode()->StartHordeMode(kHordeModeLevelThree);
}

void ULevelSelectDialogWidget::OnLevelBossButtonClicked()
{
    Hide();
    GetGameMode()->StartHordeMode(kHordeModeLevelBoss);
}

void ULevelSelectDialogWidget::OnMainMenuButtonClicked()
{
    Hide();
    GetDialogManager()->ShowMainMenuDialog();
}
