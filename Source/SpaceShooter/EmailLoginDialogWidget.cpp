// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "EmailLoginDialogWidget.h"
#include "DialogManager.h"
#include "NetworkManager.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"


bool UEmailLoginDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    LoginButton->OnClicked.AddUniqueDynamic(this, &UEmailLoginDialogWidget::OnLoginButtonClicked);
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);
    
    if(AnonymousButton != nullptr)
        AnonymousButton->OnClicked.AddUniqueDynamic(this, &UEmailLoginDialogWidget::OnAnonymousButtonClicked);
    
    if(TwitchButton != nullptr)
        TwitchButton->OnClicked.AddUniqueDynamic(this, &UEmailLoginDialogWidget::OnTwitchButtonClicked);

    return true;
}

void UEmailLoginDialogWidget::OnClose()
{
    // Dialog closed without logging in, show the main menu dialog
    GetDialogManager()->ShowMainMenuDialog();
}

UDialogWidget::Type UEmailLoginDialogWidget::GetType()
{
    return UDialogWidget::Type::EmailLogin;
}

void UEmailLoginDialogWidget::OnLoginButtonClicked()
{
    Hide();
    GetDialogManager()->ShowConnectingDialog();
    GetNetworkManager()->RequestEmailAuthentication(EmailInputBox->GetText().ToString(), PasswordInputBox->GetText().ToString());
}

void UEmailLoginDialogWidget::OnAnonymousButtonClicked()
{
    Hide();
    GetDialogManager()->ShowConnectingDialog();
    GetNetworkManager()->RequestAnonymousAuthentication();
}

void UEmailLoginDialogWidget::OnTwitchButtonClicked()
{
    Hide();
    GetDialogManager()->ShowConnectingDialog();
    GetNetworkManager()->RequestTwitchExternalAuthentication();
}
