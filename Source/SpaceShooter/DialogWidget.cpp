// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "DialogWidget.h"
#include "DialogManager.h"
#include "NetworkManager.h"
#include "SpaceShooterGameModeBase.h"
#include "SpaceShooterPlayerController.h"


bool UDialogWidget::Initialize()
{    
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    m_Callback = nullptr;
    m_IsShowing = false;
    
    return true;
}

void UDialogWidget::Show(bool triggerCallback)
{
    m_IsShowing = true;
    AddToViewport(1);

    SetVisibility(ESlateVisibility::Visible);

    OnShow();

    if (triggerCallback && m_Callback != nullptr)
        m_Callback->OnDialogShown(this);
}

void UDialogWidget::Hide(bool triggerCallback)
{
    m_IsShowing = false;
    RemoveFromViewport();
    
    SetVisibility(ESlateVisibility::Hidden);

    OnHide();

    if(triggerCallback && m_Callback != nullptr)
        m_Callback->OnDialogHidden(this);
}

bool UDialogWidget::IsShowing()
{
    return m_IsShowing;
}

void UDialogWidget::SetCallback(DialogWidgetCallback* callback)
{
    m_Callback = callback;
}

UDialogWidget::Type UDialogWidget::GetType()
{
    return UDialogWidget::Type::Unknown;
}

void UDialogWidget::OnCloseButtonClicked()
{
    Hide();
    OnClose();
}

ASpaceShooterGameModeBase* UDialogWidget::GetGameMode() const
{
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    check(gameMode != nullptr);
    return gameMode;
}

UDialogManager* UDialogWidget::GetDialogManager() const
{
    return GetGameMode()->GetDialogManager();
}

UNetworkManager* UDialogWidget::GetNetworkManager() const
{
    return GetGameMode()->GetNetworkManager();
}

void UDialogWidget::OnShow()
{
    
}

void UDialogWidget::OnHide()
{
    

}

void UDialogWidget::OnClose()
{
    
}
