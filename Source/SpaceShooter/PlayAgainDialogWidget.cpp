// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "PlayAgainDialogWidget.h"
#include "DialogManager.h"
#include "SpaceShooterGameModeBase.h"
#include "Components/Button.h"


bool UPlayAgainDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;

    NewGameButton->OnClicked.AddUniqueDynamic(this, &UPlayAgainDialogWidget::OnNewGameButtonClicked);
    BrainCloudButton->OnClicked.AddUniqueDynamic(this, &UPlayAgainDialogWidget::OnBrainCloudButtonClicked);
    MainMenuButton->OnClicked.AddUniqueDynamic(this, &UPlayAgainDialogWidget::OnMainMenuButtonClicked);
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);

    return true;
}

UDialogWidget::Type UPlayAgainDialogWidget::GetType()
{
    return UDialogWidget::Type::PlayAgain;
}

void UPlayAgainDialogWidget::OnNewGameButtonClicked()
{
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    gameMode->ResetGame(true);
    
    Hide();
}

void UPlayAgainDialogWidget::OnBrainCloudButtonClicked()
{
    GetDialogManager()->ShowBrainCloudDialog(true);
}

void UPlayAgainDialogWidget::OnMainMenuButtonClicked()
{
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    gameMode->ResetGame(false);
    
    Hide();
    GetDialogManager()->ShowMainMenuDialog();
}
