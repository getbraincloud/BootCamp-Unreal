// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "PostScoreDialogWidget.h"
#include "DialogManager.h"
#include "NetworkManager.h"
#include "Constants.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"


bool UPostScoreDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    m_Time = 0.0f;
    
    SubmitButton->OnClicked.AddUniqueDynamic(this, &UPostScoreDialogWidget::OnSubmitButtonClicked);
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);
    
    return true;
}

UDialogWidget::Type UPostScoreDialogWidget::GetType()
{
    return UDialogWidget::Type::SubmitScore;
}

void UPostScoreDialogWidget::Set(float time)
{
    m_Time = time;
}

void UPostScoreDialogWidget::OnSubmitButtonClicked()
{
    Hide();
    GetNetworkManager()->PostScoreToAllLeaderboards(m_Time, NameInputBox->GetText().ToString());
    GetNetworkManager()->UpdateUsername(NameInputBox->GetText().ToString());
}
