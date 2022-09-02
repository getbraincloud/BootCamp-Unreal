// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "Boss.h"
#include "Constants.h"
#include "Spawner.h"
#include "ShipWing.h"
#include "SpaceShooterGameModeBase.h"
#include "PaperSprite.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
ABoss::ABoss() :
    m_Callback(nullptr),
    m_SmallMissileDelay(0.0f),
    m_BigMissileDelay(0.0f),
    m_MovementDelay(0.0f),
    m_State(State::Unknown),
    m_LastMovementDirection(MovementDirection::Unknown)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UPaperSprite> spriteAsset(TEXT("/Game/Sprites/Boss_Sprite"));
    if (spriteAsset.Succeeded())
        m_Sprite->SetSprite(spriteAsset.Object);
}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
    SetDepth(kBossLayer);
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    float timeScale = GetGameMode()->GetTimeScale();

    if (m_Slider.IsSliding())
    {
        m_Slider.Tick(DeltaTime * timeScale);
        SetPosition(m_Slider.GetCurrent());
    }
    
    if (m_State == State::Attacking)
    {
        if (m_MovementDelay > 0.0f)
        {
            m_MovementDelay -= DeltaTime * timeScale;

            if (m_MovementDelay < 0.0f)
                SetupNextMovement();
        }

        // Clamp the position on screen
        FVector2D position = GetPosition();
        position.X = FMath::Clamp(position.X, kBossMinX, kScreenWidth - kHudHeight - GetSpriteSize().X * 0.5f);
        position.Y = FMath::Clamp(position.Y, GetSpriteSize().Y * 0.5f, kScreenHeight - kHudHeight - GetSpriteSize().Y * 0.5f);
        SetPosition(position);
        
        if (m_SmallMissileDelay > 0.0f)
        {
            m_SmallMissileDelay -= DeltaTime * timeScale;

            if (m_SmallMissileDelay <= 0.0f)
            {
                m_SmallMissileDelay = 0.0f;
                FireMissile(AMissile::Size::Small);
            }
        }

        if (m_BigMissileDelay > 0.0f)
        {
            m_BigMissileDelay -= DeltaTime * timeScale;

            if (m_BigMissileDelay <= 0.0f)
            {
                m_BigMissileDelay = 0.0f;
                FireMissile(AMissile::Size::Big);
            }
        }

    }
}

void ABoss::NotifyActorBeginOverlap(class AActor* otherActor)
{
    Super::NotifyActorBeginOverlap(otherActor);
        
    if (IsActive() && GetName().Contains("Boss"))
    {
        if (otherActor->GetName().Contains("Asteroid"))
        {
            // Asteroids don't damage the boss, they explode on impact
            AAsteroid* asteroid = Cast<AAsteroid>(otherActor);
            if(asteroid->IsActive())
                asteroid->Explode();
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
                        
                        Statistic* enemiesKilledStat = GetGameMode()->GetStatisticsManager()->GetStatisticByName(kBrainCloudStatEnemiesKilled);
                        if (enemiesKilledStat != nullptr)
                            enemiesKilledStat->ApplyIncrement();
                    }

                    laser->Deactivate();
                }
            }
        }
    }
}

void ABoss::Spawn()
{
    m_State = State::Spawning;
    m_LastMovementDirection = MovementDirection::Unknown;

    FVector2D start(kBossOffScreenSpawnX, (kScreenHeight - kHudHeight) * 0.5f);
    FVector2D target(kBossSpawnX, (kScreenHeight - kHudHeight) * 0.5f);
    m_Slider.StartSlide(start, target, 1.0f, this);

    SetPosition(start);
    SetRadians(PI);
    
    ResetMissileDelay(AMissile::Size::Small);
    ResetMissileDelay(AMissile::Size::Big);

    SetHealth(kBossHealth);
    
    Activate();
}

void ABoss::Explode()
{
    if (IsActive())
    {
        m_State = State::Dead;
        
        FVector2D position = GetPosition();
        float radians = GetRadians();
        
        GetSpawner()->SpawnExplosion(position, FVector2D(4.0f, 4.0f));

        // Front wings
        float leftFrontOffset = PI * 0.75f;
        AShipWing* leftFrontWing = GetSpawner()->SpawnBossWing(position, radians, leftFrontOffset, kBossFrontLeftWingAtlasKey);
        if (leftFrontWing)
            leftFrontWing->FadeOut(kShipWingExplosionFadeOutTime);

        float rightFrontOffset = -PI * 0.75f;
        AShipWing* rightFrontWing = GetSpawner()->SpawnBossWing(position, radians, rightFrontOffset, kBossFrontRightWingAtlasKey);
        if (rightFrontWing)
            rightFrontWing->FadeOut(kShipWingExplosionFadeOutTime);

        // Middle wings
        float leftMiddleOffset = PI * 0.5f;
        AShipWing* leftMiddleWing = GetSpawner()->SpawnBossWing(position, radians, leftMiddleOffset, kBossMiddleLeftWingAtlasKey);
        if (leftMiddleWing)
            leftMiddleWing->FadeOut(kShipWingExplosionFadeOutTime);

        float rightMiddleOffset = -PI * 0.5f;
        AShipWing* rightMiddleWing = GetSpawner()->SpawnBossWing(position, radians, rightMiddleOffset, kBossMiddleRightWingAtlasKey);
        if (rightMiddleWing)
            rightMiddleWing->FadeOut(kShipWingExplosionFadeOutTime);

        // Back wings
        float leftBackOffset = PI * 0.25f;
        AShipWing* leftBackWing = GetSpawner()->SpawnBossWing(position, radians, leftBackOffset, kBossBackLeftWingAtlasKey);
        if (leftBackWing)
            leftBackWing->FadeOut(kShipWingExplosionFadeOutTime);

        float rightBackOffset = -PI * 0.25f;
        AShipWing* rightBackWing = GetSpawner()->SpawnBossWing(position, radians, rightBackOffset, kBossBackRightWingAtlasKey);
        if (rightBackWing)
            rightBackWing->FadeOut(kShipWingExplosionFadeOutTime);

        Deactivate();

        if (m_Callback != nullptr)
            m_Callback->OnBossHasExploded(this);
    }
}

void ABoss::SetCallback(BossCallback* callback)
{
    m_Callback = callback;
}

void ABoss::FireMissile(AMissile::Size size)
{
    float radians = GetRadians();
    float halfPI = PI * 0.5f;
    float sixRadians = 0.1047198f; //6 degrees
    FVector position = RootComponent->GetRelativeLocation();
    FVector2D direction(FMath::Cos(radians), FMath::Sin(radians));

    if (size == AMissile::Size::Small)
    {
        FVector2D linearVelocity = direction * kMissileSmallSpeed;
        FVector2D direction2(FMath::Cos(radians + sixRadians), FMath::Sin(radians + sixRadians));
        FVector2D linearVelocity2 = direction2 * kMissileSmallSpeed;
        FVector2D direction3(FMath::Cos(radians - sixRadians), FMath::Sin(radians - sixRadians));
        FVector2D linearVelocity3 = direction3 * kMissileSmallSpeed;

        FVector2D edge1 = FVector2D(FMath::Cos(radians + halfPI), FMath::Sin(radians + halfPI)) * kBossGunOffset2;
        FVector2D position1 = FVector2D(position.X, position.Z) + direction * kBossGunOffset1 + edge1;
        GetSpawner()->SpawnMissile(position1, linearVelocity, AMissile::Size::Small);
        GetSpawner()->SpawnMissile(position1, linearVelocity2, AMissile::Size::Small);
        GetSpawner()->SpawnMissile(position1, linearVelocity3, AMissile::Size::Small);

        FVector2D edge2 = FVector2D(FMath::Cos(radians - halfPI), FMath::Sin(radians - halfPI)) * kBossGunOffset2;
        FVector2D position2 = FVector2D(position.X, position.Z) + direction * kBossGunOffset1 + edge2;
        GetSpawner()->SpawnMissile(position2, linearVelocity, AMissile::Size::Small);
        GetSpawner()->SpawnMissile(position2, linearVelocity2, AMissile::Size::Small);
        GetSpawner()->SpawnMissile(position2, linearVelocity3, AMissile::Size::Small);
    }
    else
    {
        FVector2D linearVelocity = direction * kMissileBigSpeed;
        FVector2D missilePosition = FVector2D(position.X, position.Z) + direction * kBossGunOffset1;
        GetSpawner()->SpawnMissile(missilePosition, linearVelocity, AMissile::Size::Big);
    }

    ResetMissileDelay(size);
}

void ABoss::ResetMissileDelay(AMissile::Size size)
{
    if (size == AMissile::Size::Small)
        m_SmallMissileDelay = FMath::RandRange(kBossSmallMissileMinDelay, kBossSmallMissileMaxDelay);
    else
        m_BigMissileDelay = FMath::RandRange(kEnemyLaserMinDelay, kEnemyLaserMaxDelay);
}

void ABoss::SetupNextMovement()
{
    if (m_LastMovementDirection == MovementDirection::Down)
        SetupUpMovement();
    else if (m_LastMovementDirection == MovementDirection::Up)
        SetupDownMovement();
    else
    {
        if (FMath::RandRange(0, 3) <= 1)
            SetupUpMovement();
        else
            SetupDownMovement();
    }
}

void ABoss::SetupUpMovement()
{
    m_LastMovementDirection = MovementDirection::Up;

    float radians = FMath::RandRange(0.0f, PI);
    float distance = FMath::RandRange(kBossMovementMinRange, kBossMovementMaxRange);
    float time = distance / kBossSpeed;
    
    FVector position = RootComponent->GetRelativeLocation();
    FVector2D start(position.X, position.Z);
    FVector2D target = start + FVector2D(FMath::Cos(radians) * distance, FMath::Sin(radians) * distance);

    m_Slider.StartSlide(start, target, time, this);
}

void ABoss::SetupDownMovement()
{
    m_LastMovementDirection = MovementDirection::Down;

    float radians = -FMath::RandRange(0.0f, PI);
    float distance = FMath::RandRange(kBossMovementMinRange, kBossMovementMaxRange);
    float time = distance / kBossSpeed;
    
    FVector position = RootComponent->GetRelativeLocation();
    FVector2D start(position.X, position.Z);
    FVector2D target = start + FVector2D(FMath::Cos(radians) * distance, FMath::Sin(radians) * distance);

    m_Slider.StartSlide(start, target, time, this);
}

void ABoss::OnSlideHasCompleted(class VSlider* slider)
{
    if (m_State == State::Spawning)
        m_State = State::Attacking;
    
    m_MovementDelay = FMath::RandRange(kBossMovementMinDelay, kBossMovementMaxDelay);
}
