// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "Constants.h"
#include "StatisticsDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UStatisticsDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;
    void OnShow() override;

public:
    UDialogWidget::Type GetType() override;
    
private:
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
    
    UPROPERTY(meta = (BindWidget)) class UStatisticElementUserWidget* StatisticElement_0;
    UPROPERTY(meta = (BindWidget)) class UStatisticElementUserWidget* StatisticElement_1;
    UPROPERTY(meta = (BindWidget)) class UStatisticElementUserWidget* StatisticElement_2;
    UPROPERTY(meta = (BindWidget)) class UStatisticElementUserWidget* StatisticElement_3;
    UPROPERTY(meta = (BindWidget)) class UStatisticElementUserWidget* StatisticElement_4;
    UPROPERTY(meta = (BindWidget)) class UStatisticElementUserWidget* StatisticElement_5;
    UPROPERTY(meta = (BindWidget)) class UStatisticElementUserWidget* StatisticElement_6;
    UPROPERTY(meta = (BindWidget)) class UStatisticElementUserWidget* StatisticElement_7;

    class UStatisticElementUserWidget* m_StatisticElements[kMaxStatisticElements];
};
