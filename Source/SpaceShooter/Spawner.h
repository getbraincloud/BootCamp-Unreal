// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asteroid.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Laser.h"
#include "LaserImpact.h"
#include "Missile.h"
#include "ShipWing.h"
#include "Pickup.h"
#include "Boss.h"
#include "LevelData.h"
#include "ObjectPoolComponent.h"
#include <vector>
#include "UObject/NoExportTypes.h"
#include "Spawner.generated.h"


UCLASS()
class SPACESHOOTER_API USpawner : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    USpawner();
    ~USpawner() = default;
        
	void Tick(float DeltaTime);
        
    void CreateObjectPools();
    void SetLevelData(const std::vector<LevelData>& levelData);
    
    void StartSpawning(int levelIndex);
    void StopSpawning();
    void Reset();
    
    void ExplodeAllActive();

    AAsteroid* SpawnAsteroid(AAsteroid::Size size);
    AAsteroid* SpawnAsteroid(FVector2D position, FVector2D linearVelocity, AAsteroid::Size size);

    AEnemy* SpawnEnemy(AEnemy::Type type, int health);
    AEnemy* SpawnEnemy(FVector2D position, FVector2D linearVelocity, AEnemy::Type type, int health);
    
    AExplosion* SpawnExplosion(FVector2D position, FVector2D scale = FVector2D::UnitVector, float delay = 0.0f);
    
    ALaser* SpawnBlueLaser(FVector2D position, FVector2D linearVelocity);
    ALaser* SpawnRedLaser(FVector2D position, FVector2D linearVelocity);

    ALaserImpact* SpawnLaserImpact(FVector2D position, ALaser::Color color);

    AMissile* SpawnMissile(FVector2D position, FVector2D linearVelocity, AMissile::Size size);
    
    AShipWing* SpawnBossWing(FVector2D initialPosition, float initialRadians, float offsetRadians, const FString& texture);
    AShipWing* SpawnWing(FVector2D initialPosition, float initialRadians, const FString& texture, bool isLeft);
    
    APickup* SpawnPickup(FVector2D position, FVector2D linearVelocity, APickup::Type type);

    ABoss* SpawnBoss();

private:
    AEnemy* SpawnEnemy(const FString& type, int health);
    ALaser* SpawnLaser(FVector2D position, FVector2D linearVelocity, ALaser::Color color);
    AShipWing* SpawnWing(FVector2D initialPosition, FVector2D linearVelocity, float initialRadians, const FString& texture);

    LevelData* GetCurrentLevelData();
    
    bool HandleSpawnTimer(float deltaTime, float& timer);
    
    UPROPERTY() USceneComponent* m_Root;
    UPROPERTY() UObjectPoolComponent* m_AsteroidObjectPool;
    UPROPERTY() UObjectPoolComponent* m_EnemyObjectPool;
    UPROPERTY() UObjectPoolComponent* m_ExplosionObjectPool;
    UPROPERTY() UObjectPoolComponent* m_LaserObjectPool;
    UPROPERTY() UObjectPoolComponent* m_LaserImpactObjectPool;
    UPROPERTY() UObjectPoolComponent* m_MissileObjectPool;
    UPROPERTY() UObjectPoolComponent* m_ShipWingObjectPool;
    UPROPERTY() UObjectPoolComponent* m_PickupObjectPool;
    ABoss* m_Boss;
    std::vector<LevelData> m_LevelData;
    LevelData m_DefaultLevel;
    float m_AsteroidSpawnTimer;
    float m_EnemySpawnTimer;
    int m_LevelIndex;
    bool m_CanSpawn;
};
