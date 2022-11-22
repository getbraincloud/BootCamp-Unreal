// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Statistic.h"
#include <vector>
#include "UObject/NoExportTypes.h"
#include "StatisticsManager.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UStatisticsManager : public UObject
{
	GENERATED_BODY()
	
public:
    UStatisticsManager() = default;
    ~UStatisticsManager() = default;
    
    FString GetFormatedIncrementsData();
    
    void SetStatistics(const std::vector<Statistic>& statistics);
    
    Statistic* GetStatisticByName(const FString& name);
    Statistic* GetStatisticAtIndex(int index);
    
    int GetCount();
    
private:
    std::vector<Statistic> m_Statistics;
};
