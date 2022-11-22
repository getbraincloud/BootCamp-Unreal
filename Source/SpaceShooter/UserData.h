// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SPACESHOOTER_API UserData
{
public:
    UserData(const FString& entityID, const FString& entityType, TSharedPtr<FJsonObject> entityData);
    UserData(const FString& entityID, const FString& entityType);
    UserData(const UserData& userData);
    UserData();
        
    bool SetLevelCompleted(int levelIndex);
        
    FString GetEntityID() const;
    FString GetEntityType() const;
    FString GetJsonData() const;
    
    bool HasLevelOneBeenCompleted() const;
    bool HasLevelTwoBeenCompleted() const;
    bool HasLevelThreeBeenCompleted() const;
    bool HasLevelBossBeenCompleted() const;
    
private:
    FString m_EntityID;
    FString m_EntityType;
    bool m_LevelOneCompleted;
    bool m_LevelTwoCompleted;
    bool m_LevelThreeCompleted;
    bool m_LevelBossCompleted;
};
