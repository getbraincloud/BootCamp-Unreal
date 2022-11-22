// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderboardsManager.h"

void ULeaderboardsManager::AddLeaderboard(const Leaderboard& leaderboard)
{
    // Remove all existing leaderboards with the same name
    FString name = leaderboard.GetName();
    m_Leaderboards.erase(std::remove_if(m_Leaderboards.begin(), m_Leaderboards.end(), [&, name](Leaderboard& leaderboard)-> bool{
        return leaderboard.GetName() == name;
    }), m_Leaderboards.end());
    
    // Add the Leaderboard object to the list
    m_Leaderboards.push_back(leaderboard);
    
    // Check the leaderboard for the user's time
    if (m_UserTime > 0.0f)
    {
        for (int i = 0; i < m_Leaderboards.back().GetCount(); i++)
        {
            if (m_Leaderboards.back().GetLeaderboardEntryAtIndex(i)->GetTime() == m_UserTime)
            {
                m_Leaderboards.back().GetLeaderboardEntryAtIndex(i)->SetIsUserScore(true);
                break;
            }
        }
    }
}

int ULeaderboardsManager::GetCount()
{
    return m_Leaderboards.size();
}

Leaderboard* ULeaderboardsManager::GetLeaderboardByName(const FString& name)
{
    for (int i = 0; i < m_Leaderboards.size(); i++)
        if (m_Leaderboards[i].GetName() == name)
            return &m_Leaderboards[i];
    
    return nullptr;
}

Leaderboard* ULeaderboardsManager::GetLeaderboardAtIndex(int index)
{
    if (index >= 0 && index < m_Leaderboards.size())
        return &m_Leaderboards[index];
    return nullptr;
}

void ULeaderboardsManager::SetUserTime(float userTime)
{
    long ms = (long)(userTime * 1000.0f);       // Convert the time from seconds to milleseconds
    m_UserTime = (float)(ms) / 1000.0f;
}
