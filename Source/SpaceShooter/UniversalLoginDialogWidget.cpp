// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "UniversalLoginDialogWidget.h"
#include "DialogManager.h"
#include "NetworkManager.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"


bool UUniversalLoginDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    LoginButton->OnClicked.AddUniqueDynamic(this, &UUniversalLoginDialogWidget::OnLoginButtonClicked);
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);
    
    return true;
}

UDialogWidget::Type UUniversalLoginDialogWidget::GetType()
{
    return UDialogWidget::Type::UniversalLogin;
}

void UUniversalLoginDialogWidget::OnLoginButtonClicked()
{
    Hide();
    GetDialogManager()->ShowConnectingDialog();
    GetNetworkManager()->RequestUniversalAuthentication(UsernameInputBox->GetText().ToString(), PasswordInputBox->GetText().ToString());
}

void UUniversalLoginDialogWidget::OnClose()
{
    // Dialog closed without logging in, show the main menu dialog
    GetDialogManager()->ShowMainMenuDialog();
}
