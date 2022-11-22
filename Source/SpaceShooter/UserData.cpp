// Fill out your copyright notice in the Description page of Project Settings.


#include "UserData.h"

UserData::UserData(const FString& entityID, const FString& entityType, TSharedPtr<FJsonObject> entityData) :
    m_EntityID(entityID),
    m_EntityType(entityType),
    m_LevelOneCompleted(false),
    m_LevelTwoCompleted(false),
    m_LevelThreeCompleted(false),
    m_LevelBossCompleted(false)
{
    m_LevelOneCompleted = entityData->GetBoolField(TEXT("levelOneCompleted"));
    m_LevelTwoCompleted = entityData->GetBoolField(TEXT("levelTwoCompleted"));
    m_LevelThreeCompleted = entityData->GetBoolField(TEXT("levelThreeCompleted"));
    m_LevelBossCompleted = entityData->GetBoolField(TEXT("levelBossCompleted"));
}

UserData::UserData(const FString& entityID, const FString& entityType) :
    m_EntityID(entityID),
    m_EntityType(entityType),
    m_LevelOneCompleted(false),
    m_LevelTwoCompleted(false),
    m_LevelThreeCompleted(false),
    m_LevelBossCompleted(false)
{
    
}

UserData::UserData(const UserData& userData) :
    m_EntityID(userData.m_EntityID),
    m_EntityType(userData.m_EntityType),
    m_LevelOneCompleted(userData.m_LevelOneCompleted),
    m_LevelTwoCompleted(userData.m_LevelTwoCompleted),
    m_LevelThreeCompleted(userData.m_LevelThreeCompleted),
    m_LevelBossCompleted(userData.m_LevelBossCompleted)
{
    
}

UserData::UserData() :
    m_EntityID(""),
    m_EntityType(""),
    m_LevelOneCompleted(false),
    m_LevelTwoCompleted(false),
    m_LevelThreeCompleted(false),
    m_LevelBossCompleted(false)
{
    
}

bool UserData::SetLevelCompleted(int levelIndex)
{
    bool valueChanged = false;
    
    if (levelIndex == 0)
    {
        if (m_LevelOneCompleted != true)
            valueChanged = true;
        
        m_LevelOneCompleted = true;
    }
    
    if (levelIndex == 1)
    {
        if (m_LevelTwoCompleted != true)
            valueChanged = true;
        
        m_LevelTwoCompleted = true;
    }
    
    if (levelIndex == 2)
    {
        if (m_LevelThreeCompleted != true)
            valueChanged = true;
        
        m_LevelThreeCompleted = true;
    }
    
    if (levelIndex == 3)
    {
        if (m_LevelBossCompleted != true)
            valueChanged = true;
        
        m_LevelBossCompleted = true;
    }
    
    return valueChanged;
}

FString UserData::GetEntityID() const
{
    return m_EntityID;
}

FString UserData::GetEntityType() const
{
    return m_EntityType;
}

FString UserData::GetJsonData() const
{
    FString jsonData = "{\"levelOneCompleted\":\"" + FString(m_LevelOneCompleted ? "true" : "false") +
    "\",\"levelTwoCompleted\":\"" + FString(m_LevelTwoCompleted ? "true" : "false") +
    "\",\"levelThreeCompleted\":\"" + FString(m_LevelThreeCompleted ? "true" : "false") +
    "\",\"levelBossCompleted\":\"" + FString(m_LevelBossCompleted ? "true" : "false") + "\"}";
    return jsonData;
}

bool UserData::HasLevelOneBeenCompleted() const
{
    return m_LevelOneCompleted;
}

bool UserData::HasLevelTwoBeenCompleted() const
{
    return m_LevelTwoCompleted;
}

bool UserData::HasLevelThreeBeenCompleted() const
{
    return m_LevelThreeCompleted;
}

bool UserData::HasLevelBossBeenCompleted() const
{
    return m_LevelBossCompleted;
}

