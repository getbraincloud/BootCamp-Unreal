// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SPACESHOOTER_API Achievement
{
public:
    Achievement(const FString& achievementID, const FString& title, const FString& description, const FString& status, int sortOrder);
            
    bool IsAwarded() const;
    FString GetStatusString() const;
    FString GetAchievementID() const;
    FString GetTitle() const;
    FString GetDescription() const;
    int GetSortOrder() const;
    
private:
    enum class Status
    {
        Awarded,
        NotAwarded
    };
    
    FString m_AchievementID;
    FString m_Title;
    FString m_Description;
    Status m_Status;
    int m_SortOrder;
};
