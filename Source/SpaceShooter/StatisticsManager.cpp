// Fill out your copyright notice in the Description page of Project Settings.


#include "StatisticsManager.h"


FString UStatisticsManager::GetFormatedIncrementsData()
{
    FString jsonData = TEXT("{");
    
    for (int i = 0; i < m_Statistics.size(); i++)
    {
        jsonData.Append(TEXT("\"" + m_Statistics[i].GetName() + "\" : " + FString::FromInt(m_Statistics[i].GetIncrement())));
                
        if(i != m_Statistics.size() - 1)
            jsonData.Append(TEXT(","));
    }
    
    return jsonData;
}

void UStatisticsManager::SetStatistics(const std::vector<Statistic>& statistics)
{
    m_Statistics = statistics;
}

Statistic* UStatisticsManager::GetStatisticByName(const FString& name)
{
    for (int i = 0; i < m_Statistics.size(); i++)
        if (m_Statistics[i].GetName() == name)
            return &m_Statistics[i];
    return nullptr;
}

Statistic* UStatisticsManager::GetStatisticAtIndex(int index)
{
    if (index >= 0 && index <  m_Statistics.size())
        return &m_Statistics[index];
    return nullptr;
}

int UStatisticsManager::GetCount()
{
    return m_Statistics.size();
}
