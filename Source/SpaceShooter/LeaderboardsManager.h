// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Leaderboard.h"
#include "LeaderboardEntry.h"
#include "Constants.h"
#include <vector>
#include "UObject/NoExportTypes.h"
#include "LeaderboardsManager.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API ULeaderboardsManager : public UObject
{
	GENERATED_BODY()
	
public:
    ULeaderboardsManager() = default;
    ~ULeaderboardsManager() = default;
    
    void AddLeaderboard(const Leaderboard& leaderboard);
    
    int GetCount();
    
    Leaderboard* GetLeaderboardByName(const FString& name);
    Leaderboard* GetLeaderboardAtIndex(int index);
    
    void SetUserTime(float userTime);
    
private:
    std::vector<Leaderboard> m_Leaderboards;
    float m_UserTime;
};
