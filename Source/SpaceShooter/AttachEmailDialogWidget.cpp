// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "AttachEmailDialogWidget.h"
#include "DialogManager.h"
#include "NetworkManager.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"


bool UAttachEmailDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    AttachButton->OnClicked.AddUniqueDynamic(this, &UAttachEmailDialogWidget::OnAttachButtonClicked);
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);
    
    return true;
}

UDialogWidget::Type UAttachEmailDialogWidget::GetType()
{
    return UDialogWidget::Type::AttachEmail;
}

void UAttachEmailDialogWidget::OnAttachButtonClicked()
{
    Hide();
    GetDialogManager()->ShowConnectingDialog();
    GetNetworkManager()->AttachEmailIdentity(EmailInputBox->GetText().ToString(), PasswordInputBox->GetText().ToString());
}
