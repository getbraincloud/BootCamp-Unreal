// Fill out your copyright notice in the Description page of Project Settings.


#include "Achievement.h"


Achievement::Achievement(const FString& achievementID, const FString& title, const FString& description, const FString& status, int sortOrder) :
    m_AchievementID(achievementID),
    m_Title(title),
    m_Description(description),
    m_Status(status == "AWARDED" ? Status::Awarded : Status::NotAwarded),
    m_SortOrder(sortOrder)
{
}

bool Achievement::IsAwarded() const
{
    return m_Status == Status::Awarded;
}

FString Achievement::GetStatusString() const
{
    if (m_Status == Status::Awarded)
        return TEXT("Earned");
    return TEXT("");
}

FString Achievement::GetAchievementID() const
{
    return m_AchievementID;
}

FString Achievement::GetTitle() const
{
    return m_Title;
}

FString Achievement::GetDescription() const
{
    return m_Description;
}

int Achievement::GetSortOrder() const
{
    return m_SortOrder;
}

