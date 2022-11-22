// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "ErrorDialogWidget.h"
#include "DialogManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


bool UErrorDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);

    return true;
}

void UErrorDialogWidget::OnClose()
{
    // Dialog closed without logging in, show the main menu dialog
    GetDialogManager()->ShowMainMenuDialog();
}

UDialogWidget::Type UErrorDialogWidget::GetType()
{
    return UDialogWidget::Type::Error;
}

void UErrorDialogWidget::SetMessage(const FString& errorMessage)
{
    MessageText->SetText(FText::FromString(errorMessage));
}
