// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "Enemy.h"
#include "Constants.h"
#include "ShipWing.h"
#include "Missile.h"
#include "Spawner.h"
#include "Asteroid.h"
#include "Laser.h"
#include "SpaceShooterGameModeBase.h"
#include "Ship.h"
#include "Shield.h"
#include "PaperSprite.h"
#include "Math/UnrealMathUtility.h"


AEnemy::AEnemy() : 
    m_LinearVelocity(FVector2D::ZeroVector),
    m_Type(Type::Unknown),
    m_LaserDelay(0.0f),
    m_EnemyFourFiringDuration(0.0f),
    m_EnemyFourFiringCooldown(0.0f),
    m_EnemyFiveMissileDelay(0.0f),
    m_EnemyFiveMissileCooldown(0.0f),
    m_LaserAlternate(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    //Set a default Sprite for the Enemy
    static ConstructorHelpers::FObjectFinder<UPaperSprite> spriteAsset(TEXT("/Game/Sprites/EnemyShip-1_Sprite"));
    if (spriteAsset.Succeeded())
        m_Sprite->SetSprite(spriteAsset.Object);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
    SetDepth(kEnemyLayer);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Move the enemy
    float timeScale = GetGameMode()->GetTimeScale();
    FVector2D position = Translate(m_LinearVelocity * DeltaTime * timeScale);
 
    // Only fire a laser if the front of the enemy is on-screen
    if (position.X < kScreenWidth)
    {
        bool canFire = true;

        // Logic below is used for enemy four and five's firing behaviours
        if (m_Type == Type::Four)
        {
            if (m_EnemyFourFiringCooldown > 0.0f)
            {
                m_EnemyFourFiringCooldown -= DeltaTime * timeScale;

                if (m_EnemyFourFiringCooldown < 0.0f)
                {
                    m_EnemyFourFiringCooldown = 0.0f;
                    m_EnemyFourFiringDuration = kEnemyFourFiringDuration;
                }
                else
                    canFire = false;
            }
            else
            {

                if (m_EnemyFourFiringDuration > 0.0f)
                {
                    m_EnemyFourFiringDuration -= DeltaTime * timeScale;

                    if (m_EnemyFourFiringDuration < 0.0f)
                    {
                        m_EnemyFourFiringDuration = 0.0f;
                        m_EnemyFourFiringCooldown = kEnemyFourFiringCooldown;
                        canFire = false;
                    }
                }
            }
        }
        else if (m_Type == Type::Five)
        {
            if (m_EnemyFiveMissileCooldown > 0.0f)
            {
                m_EnemyFiveMissileCooldown -= DeltaTime * timeScale;

                if (m_EnemyFiveMissileCooldown < 0.0f)
                {
                    m_EnemyFiveMissileDelay = FMath::RandRange(kEnemyFiveMissileMinDelay, kEnemyFiveMissileMaxDelay);
                    canFire = false;
                }
            }
            else
            {

                if (m_EnemyFiveMissileDelay > 0.0f)
                {
                    m_EnemyFiveMissileDelay -= DeltaTime * timeScale;

                    if (m_EnemyFiveMissileDelay < 0.0f)
                    {
                        m_EnemyFiveMissileDelay = 0.0f;
                        m_EnemyFiveMissileCooldown = FMath::RandRange(kEnemyFiveFiringMinCooldown, kEnemyFiveFiringMaxCooldown);
                        FireMissle();
                        ResetLaserDelay();
                    }
                    else
                        canFire = false;
                }
            }
        }

        if (m_LaserDelay > 0.0f && canFire)
        {
            m_LaserDelay -= DeltaTime * timeScale;

            if (m_LaserDelay <= 0.0f)
            {
                m_LaserDelay = 0.0f;
                FireLaser();
                ResetLaserDelay();
            }
        }
    }

    // Deactivate the enemy if it's gone off-screen
    if (position.X < -(GetSpriteSize().X * 0.5f) || position.Y < -(GetSpriteSize().Y * 0.5f) || position.Y > (kScreenHeight - kHudHeight) + (GetSpriteSize().Y * 0.5f))
        Deactivate();
}

void AEnemy::Spawn(FVector2D position, FVector2D linearVelocity, Type type, int health)
{
    if (type != AEnemy::Type::Unknown)
    {
        m_Type = type;
        m_LinearVelocity = linearVelocity;

        SetPosition(position);
        SetRadians(FMath::Atan2(linearVelocity.Y, linearVelocity.X));
        SetHealth(health);
        ResetLaserDelay();

        int keyIndex = (int)m_Type;
        SetSprite(kEnemyAtlasKeys[keyIndex]);

        if (m_Type == Type::Four)
        {
            m_EnemyFourFiringCooldown = kEnemyFourFiringCooldown * 2.0f;
            m_EnemyFourFiringDuration = 0.0f;
        }
        else if (m_Type == Type::Five)
        {
            m_EnemyFiveMissileCooldown = FMath::RandRange(kEnemyFiveFiringMinCooldown, kEnemyFiveFiringMaxCooldown);
        }
        
        Activate();
    }
}

void AEnemy::NotifyActorBeginOverlap(class AActor* otherActor)
{
    Super::NotifyActorBeginOverlap(otherActor);
        
    if (IsActive() && GetName().Contains("Enemy"))
    {
        if (otherActor->GetName().Contains("Asteroid"))
        {
            AAsteroid* asteroid = Cast<AAsteroid>(otherActor);
            if(asteroid->IsActive())
            {
                if (ApplyDamage(asteroid->GetAttackDamage()))
                    Explode();
                asteroid->Explode();
            }
        }
        else if (otherActor->GetName().Contains("Laser") && !otherActor->GetName().Contains("LaserImpact"))
        {
            ALaser* laser = Cast<ALaser>(otherActor);
            if(laser->IsActive())
            {
                if (laser->GetLaserColor() == ALaser::Color::Blue)
                {
                    GetSpawner()->SpawnLaserImpact(laser->GetFront(), laser->GetLaserColor());

                    if (ApplyDamage(laser->GetAttackDamage()))
                    {
                        Explode();
                    }

                    laser->Deactivate();
                }
            }
        }
    }
}

void AEnemy::Explode()
{
    if (IsActive())
    {
        GetSpawner()->SpawnExplosion(GetPosition());

        int keyIndex = (int)m_Type;

        FString leftTexture = kEnemyWingLeftAtlasKeys[keyIndex];
        AShipWing* leftWing = GetSpawner()->SpawnWing(GetPosition(), GetRadians(), leftTexture, true);
        if (leftWing)
            leftWing->FadeOut(kShipWingExplosionFadeOutTime);

        FString rightTexture = kEnemyWingRightAtlasKeys[keyIndex];
        AShipWing* rightWing = GetSpawner()->SpawnWing(GetPosition(), GetRadians(), rightTexture, false);
        if (rightWing)
            rightWing->FadeOut(kShipWingExplosionFadeOutTime);

        ASpaceShooterGameModeBase* gameMode = GetGameMode();
        int shieldHealth = 0;
        
        if(gameMode->GetShip() != nullptr && gameMode->GetShip()->GetShield() != nullptr)
            shieldHealth = gameMode->GetShip()->GetShield()->GetHealth();
        
        if (shieldHealth < kShipInitialShieldHealth && FMath::RandRange(1, 5) == 1)
            GetSpawner()->SpawnPickup(GetPosition(), m_LinearVelocity, APickup::Type::Shield);

        Deactivate();
    }
}

void AEnemy::FireMissle()
{
    if (m_Type == Type::Five)
    {
        float radians = GetRadians();
        FVector2D position = GetPosition();
        FVector2D direction(FMath::Cos(radians), FMath::Sin(radians));
        FVector2D linearVelocity = direction * kMissileSmallSpeed;
        FVector2D missilePosition = position + direction * kEnemyGunOffset1;
        GetSpawner()->SpawnMissile(missilePosition, linearVelocity, AMissile::Size::Small);
    }
}

void AEnemy::FireLaser()
{
    float halfPI = PI * 0.5f;
    FVector2D position(RootComponent->GetRelativeLocation().X, RootComponent->GetRelativeLocation().Z);
    FVector2D direction = m_LinearVelocity;
    direction.Normalize();
    FVector2D linearVelocity = direction * kLaserSpeed;
    float radians = FMath::Atan2(direction.Y, direction.X);
    
    if (m_Type == Type::One)
    {
        FVector2D laserPosition = position + direction * kEnemyGunOffset1;
        GetSpawner()->SpawnRedLaser(laserPosition, linearVelocity);
    }
    else if (m_Type == Type::Two)
    {
        FVector2D edge1(FMath::Cos(radians + halfPI) * kEnemyGunOffset2, FMath::Sin(radians + halfPI) * kEnemyGunOffset2);
        FVector2D position1 = position + direction * kEnemyGunOffset1 + edge1;
        GetSpawner()->SpawnRedLaser(position1, linearVelocity);

        FVector2D edge2(FMath::Cos(radians - halfPI)* kEnemyGunOffset2, FMath::Sin(radians - halfPI)* kEnemyGunOffset2);
        FVector2D position2 = position + direction * kEnemyGunOffset1 + edge2;
        GetSpawner()->SpawnRedLaser(position2, linearVelocity);
    }
    else if (m_Type == Type::Three)
    {
        m_LaserAlternate = !m_LaserAlternate;
        if (m_LaserAlternate)
        {
            FVector2D edge1(FMath::Cos(radians + halfPI) * kEnemyGunOffset3, FMath::Sin(radians + halfPI)* kEnemyGunOffset3);
            FVector2D position1 = position + direction * kEnemyGunOffset1 + edge1;
            GetSpawner()->SpawnRedLaser(position1, linearVelocity);
        }
        else
        {
            FVector2D edge2(FMath::Cos(radians - halfPI) * kEnemyGunOffset3, FMath::Sin(radians - halfPI) * kEnemyGunOffset3);
            FVector2D position2 = position + direction * kEnemyGunOffset1 + edge2;
            GetSpawner()->SpawnRedLaser(position2, linearVelocity);
        }
    }
    else if (m_Type == Type::Four)
    {
        m_LaserAlternate = !m_LaserAlternate;
        if (m_LaserAlternate)
        {
            FVector2D edge1(FMath::Cos(radians + halfPI) * kEnemyGunOffset4_1, FMath::Sin(radians + halfPI)* kEnemyGunOffset4_1);
            FVector2D edge4(FMath::Cos(radians - halfPI) * kEnemyGunOffset4_2, FMath::Sin(radians - halfPI) * kEnemyGunOffset4_2);
            FVector2D position1 = position + direction * kEnemyGunOffset1 + edge1;
            FVector2D position4 = position + direction * kEnemyGunOffset1 + edge4;
            
            GetSpawner()->SpawnRedLaser(position1, linearVelocity);
            GetSpawner()->SpawnRedLaser(position4, linearVelocity);
        }
        else
        {
            FVector2D edge2(FMath::Cos(radians + halfPI) * kEnemyGunOffset4_2, FMath::Sin(radians + halfPI) * kEnemyGunOffset4_2);
            FVector2D edge3(FMath::Cos(radians - halfPI) * kEnemyGunOffset4_1, FMath::Sin(radians - halfPI) * kEnemyGunOffset4_1);
            FVector2D position2 = position + direction * kEnemyGunOffset1 + edge2;
            FVector2D position3 = position + direction * kEnemyGunOffset1 + edge3;

            GetSpawner()->SpawnRedLaser(position2, linearVelocity);
            GetSpawner()->SpawnRedLaser(position3, linearVelocity);
        }
    }
    else if (m_Type == Type::Five)
    {
        FVector2D edge1(FMath::Cos(radians + halfPI) * kEnemyGunOffset5, FMath::Sin(radians + halfPI) * kEnemyGunOffset5);
        FVector2D position1 = position + direction * kEnemyGunOffset1 + edge1;
        GetSpawner()->SpawnRedLaser(position1, linearVelocity);

        FVector2D edge2(FMath::Cos(radians - halfPI) * kEnemyGunOffset5, FMath::Sin(radians - halfPI) * kEnemyGunOffset5);
        FVector2D position2 = position + direction * kEnemyGunOffset1 + edge2;
        GetSpawner()->SpawnRedLaser(position2, linearVelocity);
    }
}

void AEnemy::ResetLaserDelay()
{
    if (m_Type == Type::Three)
        m_LaserDelay = kEnemyThreeFireDelay;
    else if (m_Type == Type::Four)
        m_LaserDelay = kEnemyFourFireDelay;
    else if(m_Type == Type::Five)
        m_LaserDelay = FMath::RandRange(kEnemyFiveLaserMinDelay, kEnemyFiveLaserMaxDelay);
    else
        m_LaserDelay = FMath::RandRange(kEnemyLaserMinDelay, kEnemyLaserMaxDelay);
}
