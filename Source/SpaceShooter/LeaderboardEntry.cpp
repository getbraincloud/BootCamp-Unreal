// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderboardEntry.h"

LeaderboardEntry::LeaderboardEntry(const FString& nickname, float time, int rank) :
    m_Nickname(nickname),
    m_Time(time),
    m_Rank(rank),
    m_IsUserScore(false)
{
    
}

void LeaderboardEntry::SetNickname(const FString& nickname)
{
    m_Nickname = nickname;
}

FString LeaderboardEntry::GetNickname()
{
    return m_Nickname;
}

void LeaderboardEntry::SetTime(float time)
{
    m_Time = time;
}

float LeaderboardEntry::GetTime()
{
    return m_Time;
}

void LeaderboardEntry::SetRank(int rank)
{
    m_Rank = rank;
}

int LeaderboardEntry::GetRank()
{
    return m_Rank;
}

void LeaderboardEntry::SetIsUserScore(bool isUserScore)
{
    m_IsUserScore = isUserScore;
}

bool LeaderboardEntry::IsUserScore()
{
    return m_IsUserScore;
}
