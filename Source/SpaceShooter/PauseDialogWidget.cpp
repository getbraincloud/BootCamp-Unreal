// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "PauseDialogWidget.h"
#include "DialogManager.h"
#include "SpaceShooterGameModeBase.h"
#include "Components/Button.h"


bool UPauseDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;

    ResumeButton->OnClicked.AddUniqueDynamic(this, &UPauseDialogWidget::OnResumeButtonClicked);
    BrainCloudButton->OnClicked.AddUniqueDynamic(this, &UPauseDialogWidget::OnBrainCloudButtonClicked);
    MainMenuButton->OnClicked.AddUniqueDynamic(this, &UPauseDialogWidget::OnMainMenuButtonClicked);
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);

    return true;
}

void UPauseDialogWidget::OnShow()
{
    bool isAuthenticated = GetNetworkManager()->IsAuthenticated();
    BrainCloudButton->SetIsEnabled(isAuthenticated);
}

UDialogWidget::Type UPauseDialogWidget::GetType()
{
    return UDialogWidget::Type::Pause;
}

void UPauseDialogWidget::OnResumeButtonClicked()
{
    Hide();
}

void UPauseDialogWidget::OnBrainCloudButtonClicked()
{
    GetDialogManager()->ShowBrainCloudDialog(true);
}

void UPauseDialogWidget::OnMainMenuButtonClicked()
{
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    gameMode->ResetGame(false);
    
    Hide();
    GetDialogManager()->ShowMainMenuDialog();
}
