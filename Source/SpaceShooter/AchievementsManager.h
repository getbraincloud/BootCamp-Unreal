// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Achievement.h"
#include <vector>
#include "UObject/NoExportTypes.h"
#include "AchievementsManager.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UAchievementsManager : public UObject
{
	GENERATED_BODY()
    
public:
    UAchievementsManager() = default;
    ~UAchievementsManager() = default;
        
    void SetAchievements(const std::vector<Achievement>& achievements);
        
    Achievement* GetAchievementByID(const FString& achievementID);
    Achievement* GetAchievementAtIndex(int index);
    int GetCount();
        
private:
    std::vector<Achievement> m_Achievements;
};
