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
    //TODO: Implement email authentication
}

void UEmailLoginDialogWidget::OnAnonymousButtonClicked()
{
    //TODO: Implement anonymous authentication
}

void UEmailLoginDialogWidget::OnTwitchButtonClicked()
{
    //TODO: Implement Twitch external authentication
}
