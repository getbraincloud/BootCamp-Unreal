// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SPACESHOOTER_API LeaderboardEntry
{
public:
    LeaderboardEntry(const FString& nickname, float time, int rank);
    
    void SetNickname(const FString& nickname);
    FString GetNickname();
    
    void SetTime(float time);
    float GetTime();
    
    void SetRank(int rank);
    int GetRank();
    
    void SetIsUserScore(bool isUserScore);
    bool IsUserScore();
    
private:
    FString m_Nickname;
    float m_Time;
    int m_Rank;
    bool m_IsUserScore;
};
