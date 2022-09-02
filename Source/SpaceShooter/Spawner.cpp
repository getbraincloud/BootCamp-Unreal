// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "Spawner.h"
#include "Constants.h"
#include "SpaceShooterGameModeBase.h"
#include "Math/UnrealMathUtility.h"


USpawner::USpawner() :
    m_Root(nullptr),
    m_AsteroidObjectPool(nullptr),
    m_EnemyObjectPool(nullptr),
    m_ExplosionObjectPool(nullptr),
    m_LaserObjectPool(nullptr),
    m_LaserImpactObjectPool(nullptr),
    m_MissileObjectPool(nullptr),
    m_ShipWingObjectPool(nullptr),
    m_PickupObjectPool(nullptr),
    m_Boss(nullptr),
    m_DefaultLevel(),
    m_AsteroidSpawnTimer(0.0f),
    m_EnemySpawnTimer(0.0f),
    m_LevelIndex(-1),
    m_CanSpawn(false)
{
    m_AsteroidObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("AsteroidObjectPool"));
    m_EnemyObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("EnemyObjectPool"));
    m_ExplosionObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ExplosionObjectPool"));
    m_LaserObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("LaserObjectPool"));
    m_LaserImpactObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("LaserImpactObjectPool"));
    m_MissileObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("MissileObjectPool"));
    m_ShipWingObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("ShipWingObjectPool"));
    m_PickupObjectPool = CreateDefaultSubobject<UObjectPoolComponent>(TEXT("PickupObjectPool"));
}

void USpawner::CreateObjectPools()
{
    m_AsteroidObjectPool->CreatePool(AAsteroid::StaticClass(), kAsteroidPoolSize);
    m_EnemyObjectPool->CreatePool(AEnemy::StaticClass(), kEnemyPoolSize);
    m_ExplosionObjectPool->CreatePool(AExplosion::StaticClass(), kExplosionPoolSize);
    m_LaserObjectPool->CreatePool(ALaser::StaticClass(), kLaserPoolSize);
    m_LaserImpactObjectPool->CreatePool(ALaserImpact::StaticClass(), kLaserImpactPoolSize);
    m_MissileObjectPool->CreatePool(AMissile::StaticClass(), kMissilePoolSize);
    m_ShipWingObjectPool->CreatePool(AShipWing::StaticClass(), kShipWingPoolSize);
    m_PickupObjectPool->CreatePool(APickup::StaticClass(), kPickupPoolSize);

    UWorld* const world = GetWorld();
    if(world != nullptr)
    {
        m_Boss = world->SpawnActor<ABoss>(ABoss::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
        m_Boss->Deactivate();
        m_Boss->SetCallback(Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode()));
    }
}

void USpawner::Tick(float DeltaTime)
{
    if (!m_CanSpawn)
        return;
    
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    float timeScale = gameMode->GetTimeScale();
    
    if (HandleSpawnTimer(DeltaTime * timeScale, m_AsteroidSpawnTimer))
    {
        // Reset the asteroid spawn timer
        float minSpawnTime = GetCurrentLevelData()->GetAsteroidMinSpawnTime();
        float maxSpawnTime = GetCurrentLevelData()->GetAsteroidMaxSpawnTime();
        m_AsteroidSpawnTimer = FMath::RandRange(minSpawnTime, maxSpawnTime);

        AAsteroid::Size size = AAsteroid::Size::Unknown;
        int minSpawnCount = GetCurrentLevelData()->GetAsteroidMinSpawnCount();
        int maxSpawnCount = GetCurrentLevelData()->GetAsteroidMaxSpawnCount();
        int spawnCount = FMath::RandRange(minSpawnCount, maxSpawnCount);

        for (int i = 0; i < spawnCount; ++i)
        {
            size = FMath::RandRange(0, 4) >= 2 ? AAsteroid::Size::Big : AAsteroid::Size::Medium; //More likely to spawn a big asteroid
            SpawnAsteroid(size);
        }
    }
    
    if (HandleSpawnTimer(DeltaTime * timeScale, m_EnemySpawnTimer))
    {
        if (GetCurrentLevelData()->GetEnemyTypes().size() > 0)
        {
            // Reset the enemy spawn timer
            float minSpawnTime = GetCurrentLevelData()->GetEnemyMinSpawnTime();
            float maxSpawnTime = GetCurrentLevelData()->GetEnemyMaxSpawnTime();
            m_EnemySpawnTimer = FMath::RandRange(minSpawnTime, maxSpawnTime);

            int minSpawnCount = GetCurrentLevelData()->GetEnemyMinSpawnCount();
            int maxSpawnCount = GetCurrentLevelData()->GetEnemyMaxSpawnCount();
            int spawnCount = maxSpawnCount > 1 ? FMath::RandRange(minSpawnCount, maxSpawnCount) : 1;

            int index = 0;
            int health = 0;
            FString type;
            for (int i = 0; i < spawnCount; ++i)
            {
                index = FMath::RandRange(0, GetCurrentLevelData()->GetEnemyTypes().size()-1);
                health = GetCurrentLevelData()->GetEnemyTypes()[index].health;
                type = GetCurrentLevelData()->GetEnemyTypes()[index].type;
                SpawnEnemy(type, health);
            }
        }
    }
}

void USpawner::SetLevelData(const std::vector<LevelData>& levelData)
{
    m_LevelData = levelData;
}

void USpawner::StartSpawning(int levelIndex)
{
    if (levelIndex >= 0 && levelIndex < m_LevelData.size() || levelIndex == -1)
    {
        m_LevelIndex = levelIndex;
        m_CanSpawn = true;

        float minSpawnTime = GetCurrentLevelData()->GetAsteroidMinSpawnTime();
        float maxSpawnTime = GetCurrentLevelData()->GetAsteroidMaxSpawnTime();
        m_AsteroidSpawnTimer = FMath::RandRange(minSpawnTime, maxSpawnTime);

        minSpawnTime = GetCurrentLevelData()->GetEnemyMinSpawnTime();
        maxSpawnTime = GetCurrentLevelData()->GetEnemyMaxSpawnTime();
        m_EnemySpawnTimer = FMath::RandRange(minSpawnTime, maxSpawnTime);

        for (FString type : GetCurrentLevelData()->GetStartingAsteroids())
        {
            if (type == "Big")
                SpawnAsteroid(AAsteroid::Size::Big);
            else if (type == "Medium")
                SpawnAsteroid(AAsteroid::Size::Medium);
        }

        for (FString type : GetCurrentLevelData()->GetStartingEnemies())
        {
            if(type == "Boss")
                SpawnBoss();
            else
                SpawnEnemy(type, GetCurrentLevelData()->GetHealthForEnemyType(type));
        }
    }
}

void USpawner::StopSpawning()
{
    m_CanSpawn = false;
}

void USpawner::Reset()
{
    m_CanSpawn = false;
    
    m_Boss->Deactivate();
    m_AsteroidObjectPool->DeactivateAll();
    m_LaserObjectPool->DeactivateAll();
    m_LaserImpactObjectPool->DeactivateAll();
    m_ExplosionObjectPool->DeactivateAll();
    m_EnemyObjectPool->DeactivateAll();
    m_PickupObjectPool->DeactivateAll();
    m_ShipWingObjectPool->DeactivateAll();
    m_MissileObjectPool->DeactivateAll();

    float minSpawnTime = GetCurrentLevelData()->GetAsteroidMinSpawnTime();
    float maxSpawnTime = GetCurrentLevelData()->GetAsteroidMaxSpawnTime();
    m_AsteroidSpawnTimer = FMath::RandRange(minSpawnTime, maxSpawnTime);

    minSpawnTime = GetCurrentLevelData()->GetEnemyMinSpawnTime();
    maxSpawnTime = GetCurrentLevelData()->GetEnemyMaxSpawnTime();
    m_EnemySpawnTimer = FMath::RandRange(minSpawnTime, maxSpawnTime);
}

void USpawner::ExplodeAllActive()
{
    // Explode the enemies and the asteroids
    std::vector<ABaseActor*> activeAsteroids = m_AsteroidObjectPool->GetActiveObjects();
    for(ABaseActor* a : activeAsteroids)
        Cast<AAsteroid>(a)->Explode(true);
    
    std::vector<ABaseActor*> activeEnemies = m_EnemyObjectPool->GetActiveObjects();
    for(ABaseActor* e : activeEnemies)
        Cast<AEnemy>(e)->Explode();

    // Deactivate the pickups, lasers and missiles
    std::vector<ABaseActor*> activePickups = m_PickupObjectPool->GetActiveObjects();
    for(ABaseActor* p : activePickups)
        p->Deactivate();
    
    std::vector<ABaseActor*> activeLasers = m_LaserObjectPool->GetActiveObjects();
    for(ABaseActor* l : activeLasers)
        l->Deactivate();
    
    std::vector<ABaseActor*> activeMissiles = m_MissileObjectPool->GetActiveObjects();
    for(ABaseActor* m : activeMissiles)
        m->Deactivate();
}

AAsteroid* USpawner::SpawnAsteroid(AAsteroid::Size size)
{
    float radians = FMath::RandRange(kAsteroidMinSpawnRadians, kAsteroidMaxSpawnRadians);
    float minSpeed = GetCurrentLevelData()->GetAsteroidMinSpeed();
    float maxSpeed = GetCurrentLevelData()->GetAsteroidMaxSpeed();
    float speed = FMath::RandRange(minSpeed, maxSpeed);
    FVector2D linearVelocity(FMath::Cos(radians) * speed, FMath::Sin(radians) * speed);
    FVector2D position(kScreenWidth + kOffScreenSpawnBuffer, FMath::RandRange(0.0f, kScreenHeight));
    return SpawnAsteroid(position, linearVelocity, size);
}

AAsteroid* USpawner::SpawnAsteroid(FVector2D position, FVector2D linearVelocity, AAsteroid::Size size)
{
    AAsteroid* asteroid = Cast<AAsteroid>(m_AsteroidObjectPool->SpawnActor());
    if (asteroid)
        asteroid->Spawn(position, linearVelocity, size);
    return asteroid;
}

AEnemy* USpawner::SpawnEnemy(AEnemy::Type type, int health)
{
    float radians = FMath::RandRange(kEnemyMinSpawnAngle, kEnemyMaxSpawnAngle);
    float minSpeed = GetCurrentLevelData()->GetEnemyMinSpeed();
    float maxSpeed = GetCurrentLevelData()->GetEnemyMaxSpeed();
    float speed = FMath::RandRange(minSpeed, maxSpeed);
    FVector2D linearVelocity(FMath::Cos(radians) * speed, FMath::Sin(radians) * speed);
    FVector2D position(kScreenWidth + kOffScreenSpawnBuffer, FMath::RandRange(0.0f, kScreenHeight));
    return SpawnEnemy(position, linearVelocity, type, health);
}

AEnemy* USpawner::SpawnEnemy(FVector2D position, FVector2D linearVelocity, AEnemy::Type type, int health)
{
    AEnemy* enemy = Cast<AEnemy>(m_EnemyObjectPool->SpawnActor());
    if (enemy)
        enemy->Spawn(position, linearVelocity, type, health);
    return enemy;
}

AExplosion* USpawner::SpawnExplosion(FVector2D position, FVector2D scale, float delay)
{
    AExplosion* explosion = Cast<AExplosion>(m_ExplosionObjectPool->SpawnActor());
    if (explosion)
        explosion->Spawn(position, scale, delay);
    return explosion;
}

ALaser* USpawner::SpawnBlueLaser(FVector2D position, FVector2D linearVelocity)
{
    return SpawnLaser(position, linearVelocity, ALaser::Color::Blue);
}

ALaser* USpawner::SpawnRedLaser(FVector2D position, FVector2D linearVelocity)
{
    return SpawnLaser(position, linearVelocity, ALaser::Color::Red);
}

ALaserImpact* USpawner::SpawnLaserImpact(FVector2D position, ALaser::Color color)
{    
    ALaserImpact* laserImpact = Cast<ALaserImpact>(m_LaserImpactObjectPool->SpawnActor());
    if (laserImpact)
        laserImpact->Spawn(position, color);
    return laserImpact;
}

AMissile* USpawner::SpawnMissile(FVector2D position, FVector2D linearVelocity, AMissile::Size size)
{
    AMissile* missile = Cast<AMissile>(m_MissileObjectPool->SpawnActor());
    if (missile)
        missile->Spawn(position, linearVelocity, size);
    return missile;
}

AShipWing* USpawner::SpawnBossWing(FVector2D initialPosition, float initialRadians, float offsetRadians, const FString& texture)
{
    float radians = initialRadians + offsetRadians;
    FVector2D direction(FMath::Cos(radians), FMath::Sin(radians));
    FVector2D position = initialPosition + direction * kBossWingExplosionOffset;
    FVector2D linearVelocity = direction * kBossWingExplosionSpeed;

    return SpawnWing(position, linearVelocity, initialRadians, texture);
}

AShipWing* USpawner::SpawnWing(FVector2D initialPosition, float initialRadians, const FString& texture, bool isLeft)
{
    float speed = FMath::RandRange(kShipWingExplosionMinSpeed, kShipWingExplosionMaxSpeed);
    float halfPI = PI * 0.5f;
    float radians = initialRadians + (isLeft ? halfPI : -halfPI); // Perpendicular to the current direction
    FVector2D direction(FMath::Cos(radians), FMath::Sin(radians));
    FVector2D position = initialPosition + direction * kShipWingExplosionOffset;
    FVector2D linearVelocity = direction * speed;
    
    return SpawnWing(position, linearVelocity, initialRadians, texture);
}

APickup* USpawner::SpawnPickup(FVector2D position, FVector2D linearVelocity, APickup::Type type)
{
    APickup* pickup = Cast<APickup>(m_PickupObjectPool->SpawnActor());
    if (pickup)
        pickup->Spawn(position, linearVelocity, type);
    return pickup;
}

ABoss* USpawner::SpawnBoss()
{
    if (m_Boss != nullptr)
        m_Boss->Spawn();
    return m_Boss;
}

AEnemy* USpawner::SpawnEnemy(const FString& type, int health)
{
    if (type == "Enemy-1")
        return SpawnEnemy(AEnemy::Type::One, health);
    else if (type == "Enemy-2")
        return SpawnEnemy(AEnemy::Type::Two, health);
    else if (type == "Enemy-3")
        return SpawnEnemy(AEnemy::Type::Three, health);
    else if (type == "Enemy-4")
        return SpawnEnemy(AEnemy::Type::Four, health);
    else if (type == "Enemy-5")
        return SpawnEnemy(AEnemy::Type::Five, health);
    return nullptr;
}

ALaser* USpawner::SpawnLaser(FVector2D position, FVector2D linearVelocity, ALaser::Color color)
{
    ALaser* laser = Cast<ALaser>(m_LaserObjectPool->SpawnActor());
    if (laser)
        laser->Spawn(position, linearVelocity, color);    
    return laser;
}

AShipWing* USpawner::SpawnWing(FVector2D initialPosition, FVector2D linearVelocity, float initialRadians, const FString& texture)
{
    AShipWing* shipWing = Cast<AShipWing>(m_ShipWingObjectPool->SpawnActor());
    if (shipWing)
        shipWing->Spawn(initialPosition, linearVelocity, initialRadians, texture);
    return shipWing;
}

LevelData* USpawner::GetCurrentLevelData()
{
    if (m_LevelIndex == -1)
        return &m_DefaultLevel;
    return &m_LevelData[m_LevelIndex];
}

bool USpawner::HandleSpawnTimer(float deltaTime, float& timer)
{
    if (timer > 0.0f)
    {
        timer -= deltaTime;
        if (timer <= 0.0f)
            return true;
    }
    return false;
}

