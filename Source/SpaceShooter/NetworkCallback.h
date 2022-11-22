// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Leaderboard.h"
#include "LevelData.h"
#include "Statistic.h"
#include <vector>

/**
 * 
 */
class SPACESHOOTER_API NetworkCallback
{
public:
    NetworkCallback() = default;
    virtual ~NetworkCallback() = default;
    
    virtual void OnAuthenticationRequestCompleted() {}
    virtual void OnAuthenticationRequestFailed(const FString& errorMessage) {}
    
    virtual void OnLogOutRequestCompleted() {}
    virtual void OnLogOutRequestFailed() {}
    
    virtual void OnUpdateUsernameRequestCompleted() {}
    virtual void OnUpdateUsernameRequestFailed() {}
    
    virtual void OnLeaderboardRequestCompleted(const Leaderboard& leaderboard) {}
    virtual void OnLeaderboardRequestFailed() {}
    
    virtual void OnPostScoreToLeaderboardRequestCompleted() {}
    virtual void OnPostScoreToLeaderboardRequestFailed() {}
    
    virtual void OnGlobalEntityLevelDataRequestCompleted(const std::vector<LevelData>& levelData) {}
    virtual void OnGlobalEntityLevelDataRequestFailed() {}
    
    virtual void OnUserStatisticsRequestCompleted(const std::vector<Statistic>& statistics) {}
    virtual void OnUserStatisticsRequestFailed() {}
    
    //TODO: Add Network callback methods here
};
