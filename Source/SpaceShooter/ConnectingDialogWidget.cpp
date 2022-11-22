// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "ConnectingDialogWidget.h"
#include "Constants.h"
#include "Components/TextBlock.h"


bool UConnectingDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    m_DotsCount = 0;
    m_DotsTimer = kConnectingDialogDotsInterval;
    
    return true;
}

void UConnectingDialogWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    
    m_DotsTimer -= InDeltaTime;

    if (m_DotsTimer <= 0.0f)
    {
        m_DotsTimer += kConnectingDialogDotsInterval;

        m_DotsCount++;

        if (m_DotsCount >= 4)
            m_DotsCount = 0;

        FString dotsText = "";
        for (int i = 0; i < m_DotsCount; i++)
            dotsText += ".";
        
        Dots->SetText(FText::FromString(dotsText));
    }
}

UDialogWidget::Type UConnectingDialogWidget::GetType()
{
    return UDialogWidget::Type::Connecting;
}
