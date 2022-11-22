// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "MainMenuDialogWidget.h"
#include "DialogManager.h"
#include "SpaceShooterGameModeBase.h"
#include "Components/Button.h"


bool UMainMenuDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    EndlessModeButton->OnClicked.AddUniqueDynamic(this, &UMainMenuDialogWidget::OnEndlessModeButtonClicked);
    HordeModeButton->OnClicked.AddUniqueDynamic(this, &UMainMenuDialogWidget::OnHordeModeButtonClicked);
    BrainCloudButton->OnClicked.AddUniqueDynamic(this, &UMainMenuDialogWidget::OnBrainCloudButtonClicked);
    ExitButton->OnClicked.AddUniqueDynamic(this, &UMainMenuDialogWidget::OnExitButtonClicked);
    
    return true;
}

void UMainMenuDialogWidget::OnShow()
{
    bool isAuthenticated = GetNetworkManager()->IsAuthenticated();
    HordeModeButton->SetIsEnabled(isAuthenticated);
}

UDialogWidget::Type UMainMenuDialogWidget::GetType()
{
    return UDialogWidget::Type::MainMenu;
}

void UMainMenuDialogWidget::OnEndlessModeButtonClicked()
{
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    if(gameMode != nullptr)
    {
        Hide();
        gameMode->StartEndlessMode();
    }
}

void UMainMenuDialogWidget::OnHordeModeButtonClicked()
{
    Hide();
    GetDialogManager()->ShowLevelSelectDialog();
}

void UMainMenuDialogWidget::OnBrainCloudButtonClicked()
{
    GetDialogManager()->ShowBrainCloudDialog();
}

void UMainMenuDialogWidget::OnExitButtonClicked()
{
    FGenericPlatformMisc::RequestExit(false);
}
