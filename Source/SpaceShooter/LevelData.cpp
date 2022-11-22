// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "LevelData.h"
#include "Constants.h"


LevelData::LevelData() :
    m_EntityType("Default"),
    m_EntityID("-1"),
    m_Description("Survive as long as possible"),
    m_Duration(-1.0f),
    m_AsteroidMinSpawnCount(kAsteroidMinSpawnCount),
    m_AsteroidMaxSpawnCount(kAsteroidMaxSpawnCount),
    m_AsteroidMinSpawnTime(kAsteroidMinSpawnTime),
    m_AsteroidMaxSpawnTime(kAsteroidMaxSpawnTime),
    m_AsteroidMinSpeed(kAsteroidMinSpeed),
    m_AsteroidMaxSpeed(kAsteroidMaxSpeed),
    m_EnemyMinSpawnCount(kEnemyMinSpawnCount),
    m_EnemyMaxSpawnCount(kEnemyMaxSpawnCount),
    m_EnemyMinSpawnTime(kEnemyMinSpawnTime),
    m_EnemyMaxSpawnTime(kEnemyMaxSpawnTime),
    m_EnemyMinSpeed(kEnemyMinSpeed),
    m_EnemyMaxSpeed(kEnemyMaxSpeed)
{
    m_EnemyTypes.emplace_back("Enemy-1", 2);
    m_EnemyTypes.emplace_back("Enemy-2", 3);
    m_EnemyTypes.emplace_back("Enemy-3", 3);
    m_EnemyTypes.emplace_back("Enemy-4", 4);
    m_EnemyTypes.emplace_back("Enemy-5", 4);

    m_StartingAsteroids.push_back("Big");
    m_StartingAsteroids.push_back("Big");
    m_StartingAsteroids.push_back("Medium");
}

LevelData::LevelData(const FString& entityType, const FString& entityID, TSharedPtr<FJsonObject>& levelJson)
{
    m_EntityType = entityType;
    m_EntityID = entityID;
    
    m_Duration = static_cast<float>(levelJson->GetNumberField(TEXT("Duration")));
    m_Description = levelJson->GetStringField(TEXT("Description"));
    
    m_AsteroidMinSpawnCount = levelJson->GetIntegerField(TEXT("Asteroid_MinSpawnCount"));
    m_AsteroidMaxSpawnCount = levelJson->GetIntegerField(TEXT("Asteroid_MaxSpawnCount"));
    m_AsteroidMinSpawnTime = static_cast<float>(levelJson->GetNumberField(TEXT("Asteroid_MinSpawnTime")));
    m_AsteroidMaxSpawnTime = static_cast<float>(levelJson->GetNumberField(TEXT("Asteroid_MaxSpawnTime")));
    m_AsteroidMinSpeed = static_cast<float>(levelJson->GetNumberField(TEXT("Asteroid_MinSpeed")));
    m_AsteroidMaxSpeed = static_cast<float>(levelJson->GetNumberField(TEXT("Asteroid_MaxSpeed")));
    
    m_EnemyMinSpawnCount = levelJson->GetIntegerField(TEXT("Enemy_MinSpawnCount"));
    m_EnemyMaxSpawnCount = levelJson->GetIntegerField(TEXT("Enemy_MaxSpawnCount"));
    m_EnemyMinSpawnTime = static_cast<float>(levelJson->GetNumberField(TEXT("Enemy_MinSpawnTime")));
    m_EnemyMaxSpawnTime = static_cast<float>(levelJson->GetNumberField(TEXT("Enemy_MaxSpawnTime")));
    m_EnemyMinSpeed = static_cast<float>(levelJson->GetNumberField(TEXT("Enemy_MinSpeed")));
    m_EnemyMaxSpeed = static_cast<float>(levelJson->GetNumberField(TEXT("Enemy_MaxSpeed")));
    
    FString type;
    int health = 0;
    TArray<TSharedPtr<FJsonValue>> enemyTypes = levelJson->GetArrayField(TEXT("Enemy_Types"));
    
    for (int i = 0; i < enemyTypes.Num(); i++)
    {
        type = enemyTypes[i]->AsObject()->GetStringField(TEXT("Type"));
        health = enemyTypes[i]->AsObject()->GetIntegerField(TEXT("Health"));
        m_EnemyTypes.emplace_back(type, health);
    }
    
    TArray<TSharedPtr<FJsonValue>> startingAsteroids = levelJson->GetArrayField(TEXT("StartingAsteroids"));
    for (int i = 0; i < startingAsteroids.Num(); i++)
        m_StartingAsteroids.push_back(startingAsteroids[i]->AsString());
    
    TArray<TSharedPtr<FJsonValue>> startingEnemies = levelJson->GetArrayField(TEXT("StartingEnemies"));
    for (int i = 0; i < startingEnemies.Num(); i++)
        m_StartingEnemies.push_back(startingEnemies[i]->AsString());
}

LevelData::EnemyType::EnemyType(const FString& type, int health) :
    type(type),
    health(health)
{}

int LevelData::GetHealthForEnemyType(const FString& enemyType)
{
    for(EnemyType& et : m_EnemyTypes)
        if(et.type == enemyType)
            return et.health;

   return 1;
}

FString LevelData::GetEntityType() const
{
    return m_EntityType;
}

FString LevelData::GetEntityID() const
{
    return m_EntityID;
}

FString LevelData::GetDescription() const
{
    return m_Description;
}

float LevelData::GetDuration() const
{
    return m_Duration;
}

int LevelData::GetAsteroidMinSpawnCount() const
{
    return m_AsteroidMinSpawnCount;
}

int LevelData::GetAsteroidMaxSpawnCount() const
{
    return m_AsteroidMaxSpawnCount;
}

float LevelData::GetAsteroidMinSpawnTime() const
{
    return m_AsteroidMinSpawnTime;
}

float LevelData::GetAsteroidMaxSpawnTime() const
{
    return m_AsteroidMaxSpawnTime;
}

float LevelData::GetAsteroidMinSpeed() const
{
    return m_AsteroidMinSpeed;
}

float LevelData::GetAsteroidMaxSpeed() const
{
    return m_AsteroidMaxSpeed;
}

int LevelData::GetEnemyMinSpawnCount() const
{
    return m_EnemyMinSpawnCount;
}

int LevelData::GetEnemyMaxSpawnCount() const
{
    return m_EnemyMaxSpawnCount;
}

float LevelData::GetEnemyMinSpawnTime() const
{
    return m_EnemyMinSpawnTime;
}

float LevelData::GetEnemyMaxSpawnTime() const
{
    return m_EnemyMaxSpawnTime;
}

float LevelData::GetEnemyMinSpeed() const
{
    return m_EnemyMinSpeed;
}

float LevelData::GetEnemyMaxSpeed() const
{
    return m_EnemyMaxSpeed;
}

std::vector<LevelData::EnemyType> LevelData::GetEnemyTypes()
{
    return m_EnemyTypes;
}

std::vector<FString> LevelData::GetStartingAsteroids()
{
    return m_StartingAsteroids;
}

std::vector<FString> LevelData::GetStartingEnemies()
{
    return m_StartingEnemies;
}
