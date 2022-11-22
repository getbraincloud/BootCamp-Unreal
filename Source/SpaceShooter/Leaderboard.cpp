// Fill out your copyright notice in the Description page of Project Settings.


#include "Leaderboard.h"

Leaderboard::Leaderboard(const FString& name, std::vector<LeaderboardEntry>& leaderboardEntries) :
    m_LeaderboardEntries(leaderboardEntries),
    m_Name(name)
{
    
}

FString Leaderboard::GetName() const
{
    return m_Name;
}

LeaderboardEntry* Leaderboard::GetLeaderboardEntryAtIndex(int index)
{
    if (index >= 0 && index < m_LeaderboardEntries.size())
        return &m_LeaderboardEntries[index];
    return nullptr;
}

int Leaderboard::GetCount() const
{
    return m_LeaderboardEntries.size();
}
