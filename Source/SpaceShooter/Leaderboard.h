// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeaderboardEntry.h"
#include <vector>

/**
 * 
 */
class SPACESHOOTER_API Leaderboard
{
public:
    Leaderboard(const FString& name, std::vector<LeaderboardEntry>& leaderboardEntries);
    
    FString GetName() const;
    
    LeaderboardEntry* GetLeaderboardEntryAtIndex(int index);
    int GetCount() const;
    
private:
    std::vector<LeaderboardEntry> m_LeaderboardEntries;
    FString m_Name;
};
