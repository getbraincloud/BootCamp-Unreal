// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "StatisticsDialogWidget.h"
#include "StatisticElementUserWidget.h"
#include "SpaceShooterGameModeBase.h"
#include "Components/Button.h"


bool UStatisticsDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    m_StatisticElements[0] = StatisticElement_0;
    m_StatisticElements[1] = StatisticElement_1;
    m_StatisticElements[2] = StatisticElement_2;
    m_StatisticElements[3] = StatisticElement_3;
    m_StatisticElements[4] = StatisticElement_4;
    m_StatisticElements[5] = StatisticElement_5;
    m_StatisticElements[6] = StatisticElement_6;
    m_StatisticElements[7] = StatisticElement_7;
    
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);
    
    return true;
}

void UStatisticsDialogWidget::OnShow()
{
    for(int i = 0; i < kMaxStatisticElements; i++)
        m_StatisticElements[i]->Reset();
}

UDialogWidget::Type UStatisticsDialogWidget::GetType()
{
    return UDialogWidget::Type::Statistics;
}
