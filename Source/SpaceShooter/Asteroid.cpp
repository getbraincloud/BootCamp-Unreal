// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "Asteroid.h"
#include "Constants.h"
#include "Spawner.h"
#include "Laser.h"
#include "SpaceShooterGameModeBase.h"
#include "PaperSprite.h"
#include "Components/SphereComponent.h"
#include "Math/UnrealMathUtility.h"


AAsteroid::AAsteroid() :
    m_LinearVelocity(FVector2D::ZeroVector),
    m_AngularVelocity(PI / 2.0f),
    m_SpinDirection(1.0f),
    m_Size(Size::Unknown)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UPaperSprite> spriteAsset(TEXT("/Game/Sprites/AsteroidBig-1_Sprite"));
    if (spriteAsset.Succeeded())
        m_Sprite->SetSprite(spriteAsset.Object);
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
    SetDepth(kAsteroidLayer);
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    float timeScale = GetGameMode()->GetTimeScale();
    FVector2D position = Translate(m_LinearVelocity * DeltaTime * timeScale);

    float radians = GetRadians();
    
    radians += m_AngularVelocity * m_SpinDirection * DeltaTime * timeScale;
    if (m_SpinDirection > 0.0f && radians > PI * 2.0f)
        radians -= PI * 2.0f;
    else if (m_SpinDirection < 0.0f && radians < -(PI * 2.0f))
        radians += PI * 2.0f;
    
    SetRadians(radians);
    
    // If the asteroid goes off-screen deactivate it
    if (position.X < -(GetSpriteSize().X * 0.5f) || position.Y < -(GetSpriteSize().Y * 0.5f) || position.Y > (kScreenHeight - kHudHeight) + (GetSpriteSize().Y * 0.5f))
        Deactivate();
}

void AAsteroid::NotifyActorBeginOverlap(class AActor* otherActor)
{
    if (IsActive() && !IsFadingOut())
    {
        if (otherActor->GetName().Contains("Laser") && !otherActor->GetName().Contains("LaserImpact"))
        {
            ALaser* laser = Cast<ALaser>(otherActor);
            GetSpawner()->SpawnLaserImpact(laser->GetFront(), laser->GetLaserColor());
            ALaser::Color laserColor = laser->GetLaserColor();
            
            if (ApplyDamage(laser->GetAttackDamage()))
            {
                laser->Deactivate();
                Explode();
                
                if(laserColor == ALaser::Color::Blue)
                {
                    Statistic* asteroidDestroyedStat = GetGameMode()->GetStatisticsManager()->GetStatisticByName(kBrainCloudStatAsteroidDestroyed);
                    if (asteroidDestroyedStat != nullptr)
                        asteroidDestroyedStat->ApplyIncrement();
                }
            }
        }
    }
}

AAsteroid::Size AAsteroid::GetSize()
{
    return m_Size;
}

void AAsteroid::Spawn(FVector2D position, FVector2D linearVelocity, Size size)
{
    if(size != Size::Unknown)
    {
        ResetFade();
        
        SetPosition(position);

        m_LinearVelocity = linearVelocity;
        m_AngularVelocity = FMath::RandRange(kAsteroidMinAngularVelocity, kAsteroidMaxAngularVelocity);
        m_SpinDirection = FMath::RandRange(0, 3) <= 1 ? 1.0f : -1.0f;
        m_Size = size;

        int sizeValue = static_cast<int>(m_Size);
        SetHealth(kAsteroidHealth[sizeValue]);
        SetAttackDamage(kAsteroidAttackDamage[sizeValue]);

        // Determine the atlas frame to use base on the size of the asteroid
        int keyIndex = FMath::RandRange(0, kNumAsteroidVariations[sizeValue]-1);

        switch (m_Size)
        {
            case Size::Big:
                {
                    SetSprite(kBigAsteroidAtlasKeys[keyIndex]);
                }
                break;

            case Size::Medium:
                {
                    SetSprite(kMediumAsteroidAtlasKeys[keyIndex]);
                }
                break;

            case Size::Small:
                {
                    SetSprite(kSmallAsteroidAtlasKeys[keyIndex]);
                }
                break;

            case Size::Tiny:
                {
                    SetSprite(kTinyAsteroidAtlasKeys[keyIndex]);
                }
                break;

            default:
                {
                    Deactivate();
                    return;
                }
        }
        
        Activate();
    }
}

void AAsteroid::Explode(bool onlySmallDebris)
{
    //int sizeValue = static_cast<int>(m_Size);
    //UE_LOG(LogTemp, Warning, TEXT("AAsteroid::Explode() %i"), sizeValue);
    
    if (IsActive() && (m_Size == Size::Big || m_Size == Size::Medium))
    {
        FVector2D position = GetPosition();
        FVector2D linearVelocity;
        Size size = Size::Unknown;

        int numDirections = m_Size == Size::Big ? 6 : 3;
        float directionsIncrement = (PI * 2.0f) / numDirections;
        float offset = FMath::RandRange(0.0f, PI * 0.5f);
        float radians = 0.0f;
        float speed = 0.0f;

        // Spawn little asteroids as a result of the explosion
        for (int i = 0; i < numDirections; ++i)
        {
            if (onlySmallDebris)
                size = FMath::RandRange(0, 3) <= 1 ? Size::Small : Size::Tiny;
            else
            {
                if (m_Size == Size::Big)
                    size = FMath::RandRange(0, 4) > 2 ? Size::Medium : Size::Small;
                else
                    size = FMath::RandRange(0, 3) <= 1 ? Size::Small : Size::Tiny;
            }

            radians = (directionsIncrement * i) + offset;
            speed = FMath::RandRange(kAsteroidExplosionMinSpeed, kAsteroidExplosionMaxSpeed);
            linearVelocity.X = FMath::Cos(radians) * speed;
            linearVelocity.Y = FMath::Sin(radians) * speed;

            AAsteroid* asteroid = GetSpawner()->SpawnAsteroid(position, linearVelocity, size);
            if (asteroid && size != Size::Medium)
                asteroid->FadeOut(kAsteroidExplosionFadeOutTime, kAsteroidExplosionFadeDelay);
        }

        // Spawn an explosion
        float scaleF = m_Size == Size::Big ? 1.0f : 0.6f;
        GetSpawner()->SpawnExplosion(position, FVector2D(scaleF, scaleF));

        // Lasterly deactivate the asteroid
        Deactivate();
    }
}

