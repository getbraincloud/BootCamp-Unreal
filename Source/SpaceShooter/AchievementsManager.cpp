// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementsManager.h"

void UAchievementsManager::SetAchievements(const std::vector<Achievement>& achievements)
{
    m_Achievements = achievements;
}

Achievement* UAchievementsManager::GetAchievementByID(const FString& achievementID)
{
    for(int i = 0; i < m_Achievements.size(); i++)
        if (m_Achievements[i].GetAchievementID() == achievementID)
            return &m_Achievements[i];
    return nullptr;
}

Achievement* UAchievementsManager::GetAchievementAtIndex(int index)
{
    if (index >= 0 && index < GetCount())
        return &m_Achievements[index];
    return nullptr;
}

int UAchievementsManager::GetCount()
{
    return m_Achievements.size();
}
