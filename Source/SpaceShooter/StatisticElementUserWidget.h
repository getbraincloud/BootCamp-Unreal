// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Statistic.h"
#include "Blueprint/UserWidget.h"
#include "StatisticElementUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UStatisticElementUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void Reset();
    void SetStatistic(Statistic* statistic);
    
private:
    UPROPERTY(meta = (BindWidget)) class UTextBlock* NameText;
    UPROPERTY(meta = (BindWidget)) class UTextBlock* ValueText;
};
