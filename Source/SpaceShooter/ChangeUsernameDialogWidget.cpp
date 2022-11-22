// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "ChangeUsernameDialogWidget.h"
#include "NetworkManager.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"


bool UChangeUsernameDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    ChangeButton->OnClicked.AddUniqueDynamic(this, &UChangeUsernameDialogWidget::OnChangeButtonClicked);
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);
    
    return true;
}

UDialogWidget::Type UChangeUsernameDialogWidget::GetType()
{
    return UDialogWidget::Type::ChangeUsername;
}

void UChangeUsernameDialogWidget::OnChangeButtonClicked()
{
    GetNetworkManager()->UpdateUsername(UsernameInputBox->GetText().ToString());
    Hide();
}
