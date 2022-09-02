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
