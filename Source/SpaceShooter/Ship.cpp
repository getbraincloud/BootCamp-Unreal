// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "Ship.h"
#include "Constants.h"
#include "SpaceShooterGameModeBase.h"
#include "DialogManager.h"
#include "Spawner.h"
#include "ShipWing.h"
#include "Shield.h"
#include "Asteroid.h"
#include "Enemy.h"
#include "Boss.h"
#include "Laser.h"
#include "Missile.h"
#include "Pickup.h"
#include "PaperSprite.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AShip::AShip() :
    m_Callback(nullptr),
    m_Shield(nullptr),
    m_Acceleration(FVector2D::ZeroVector),
    m_LinearVelocity(FVector2D::ZeroVector),
    m_SpriteSize(FVector2D::ZeroVector),
    m_InvincibilityTimer(0.0f),
    m_Depth(0.0f),
    m_Radians(0.0f),
    m_Health(0),
    m_AttackDamage(0),
    m_IsSpawning(false),
    m_IsActive(true)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_Root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(m_Root);
    
    m_Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
    m_Sprite->SetSimulatePhysics(false);
    
    m_Sprite->GetBodyInstance()->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
    m_Sprite->GetBodyInstance()->SetObjectType( ECollisionChannel::ECC_Pawn );
    m_Sprite->GetBodyInstance()->SetResponseToChannel( ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap );
    
    m_Sprite->SetupAttachment(m_Root);
    
    static ConstructorHelpers::FObjectFinder<UPaperSprite> spriteAsset(TEXT("/Game/Sprites/Ship_Sprite"));
    if (spriteAsset.Succeeded())
        m_Sprite->SetSprite(spriteAsset.Object);
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
    
    SetHealth(kShipInitialHealth);
    SetDepth(kEnemyLayer);
    
    UPaperSprite* sprite = m_Sprite->GetSprite();
    if(sprite != nullptr)
        m_SpriteSize = FVector2D(sprite->GetBakedTexture()->GetSizeX(), sprite->GetBakedTexture()->GetSizeY());
    
    // Create and Attach the Shield to the Ship
    m_Shield = GetWorld()->SpawnActor<AShield>(AShield::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    m_Shield->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Shield"));
    
    Deactivate();
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    float timeScale = GetGameMode()->GetTimeScale();
    
    if (m_IsSpawning)
    {
       if (m_SpawnSlider.IsSliding())
       {
           m_SpawnSlider.Tick(DeltaTime * timeScale);
           SetPosition(m_SpawnSlider.GetCurrent());
       }
       return;
    }

    if (m_InvincibilityTimer > 0.0f)
    {
       m_InvincibilityTimer -= DeltaTime * timeScale;
       if (m_InvincibilityTimer <= 0.0f)
       {
           m_InvincibilityTimer = 0.0f;
           SetAlpha(1.0f);
       }
       else
           SetAlpha(kShipInvincibleAlpha);
    }

    FVector2D drag;
    drag.X = m_Acceleration.X != 0.0f ? (m_Acceleration.X - (m_Acceleration.X * kShipDrag)) / m_Acceleration.X : -kShipDrag;
    drag.Y = m_Acceleration.Y != 0.0f ? (m_Acceleration.Y - (m_Acceleration.Y * kShipDrag)) / m_Acceleration.Y : -kShipDrag;
    m_LinearVelocity += m_Acceleration + (m_LinearVelocity * drag) * FMath::Pow(DeltaTime * timeScale, 0.5f);

    // Cap the speed
    if (FMath::Abs(m_LinearVelocity.Size() > kShipMaxSpeed))
    {
        FVector2D normal = m_LinearVelocity;
        normal.Normalize();
        m_LinearVelocity = normal * kShipMaxSpeed;
    }

    // Move the ship
    FVector2D position = Translate(m_LinearVelocity * DeltaTime * timeScale);

    // Clamp the position on screen
    position.X = FMath::Clamp(position.X, kShipMinX, kScreenWidth - kShipMaxOffsetX);
    position.Y = FMath::Clamp(position.Y, 0.0f, kScreenHeight - kHudHeight);
    SetPosition(position);

    // Tilt the ship when moving along the y-axis
    float radians = GetRadians();
    radians = kShipTurnTilt * (m_LinearVelocity.Y / kShipMaxSpeed);
    SetRadians(radians);
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShip::OnFire);
    
    PlayerInputComponent->BindAction("Up", IE_Pressed, this, &AShip::OnMovementUpStart);
    PlayerInputComponent->BindAction("Up", IE_Released, this, &AShip::OnMovementUpStop);
    
    PlayerInputComponent->BindAction("Down", IE_Pressed, this, &AShip::OnMovementDownStart);
    PlayerInputComponent->BindAction("Down", IE_Released, this, &AShip::OnMovementDownStop);

    PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AShip::OnMovementLeftStart);
    PlayerInputComponent->BindAction("Left", IE_Released, this, &AShip::OnMovementLeftStop);
    
    PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AShip::OnMovementRightStart);
    PlayerInputComponent->BindAction("Right", IE_Released, this, &AShip::OnMovementRightStop);
    
    PlayerInputComponent->BindAction("Cheat_AddTime", IE_Pressed, GetGameMode(), &ASpaceShooterGameModeBase::OnCheatAddTime);
    PlayerInputComponent->BindAction("Cheat_HealUp", IE_Pressed, GetGameMode(), &ASpaceShooterGameModeBase::OnCheatHealUp);
    PlayerInputComponent->BindAction("Cheat_NextLevel", IE_Pressed, GetGameMode(), &ASpaceShooterGameModeBase::OnCheatNextLevel);

    if(GetGameMode()->GetDialogManager() != nullptr)
        PlayerInputComponent->BindAction("Escape", IE_Pressed, GetGameMode()->GetDialogManager(), &UDialogManager::OnEscape);
}

void AShip::NotifyActorBeginOverlap(AActor* otherActor)
{
    Super::NotifyActorBeginOverlap(otherActor);

    if(GetHealth() > 0)
    {
        if (otherActor->GetName().Contains("Asteroid"))
        {
            AAsteroid* asteroid = Cast<AAsteroid>(otherActor);
            if(asteroid->IsActive())
            {
                int asteroidAttackDamage = asteroid->GetAttackDamage();

                Statistic* asteroidDestroyedStat = GetGameMode()->GetStatisticsManager()->GetStatisticByName(kBrainCloudStatAsteroidDestroyed);
                if (asteroidDestroyedStat != nullptr)
                    asteroidDestroyedStat->ApplyIncrement();
                
                asteroid->Explode(true);

                if (ApplyDamage(asteroidAttackDamage))
                    Explode();
            }
        }
        else if (otherActor->GetName().Contains("Enemy"))
        {
            AEnemy* enemy = Cast<AEnemy>(otherActor);
            if(enemy->IsActive())
            {
                int enemyAttackDamage = enemy->GetAttackDamage();

                Statistic* enemiesKilledStat = GetGameMode()->GetStatisticsManager()->GetStatisticByName(kBrainCloudStatEnemiesKilled);
                if (enemiesKilledStat != nullptr)
                    enemiesKilledStat->ApplyIncrement();
                
                enemy->Explode();

                if (ApplyDamage(enemyAttackDamage))
                    Explode();
            }
        }
        else if (otherActor->GetName().Contains("Boss"))
        {
            ABoss* boss = Cast<ABoss>(otherActor);
            if(boss->IsActive())
            {
                if (ApplyDamage(boss->GetAttackDamage()))
                    Explode();
            }
        }
        else if (otherActor->GetName().Contains("Laser") && !otherActor->GetName().Contains("LaserImpact"))
        {
            ALaser* laser = Cast<ALaser>(otherActor);
            if(laser->IsActive())
            {
                if (laser->GetLaserColor() == ALaser::Color::Red)
                {
                    GetSpawner()->SpawnLaserImpact(laser->GetFront(), laser->GetLaserColor());

                    if (ApplyDamage(laser->GetAttackDamage()))
                        Explode();

                    laser->Deactivate();
                }
            }
        }
        else if (otherActor->GetName().Contains("Missile"))
        {
            AMissile* missile = Cast<AMissile>(otherActor);
            if(missile->IsActive())
            {
                if(missile->GetMissleSize() == AMissile::Size::Small)
                    GetSpawner()->SpawnExplosion(missile->GetMiddle(), FVector2D(0.2f, 0.2f));
                else
                    GetSpawner()->SpawnExplosion(missile->GetMiddle(), FVector2D(0.45f, 0.45f));

                if (ApplyDamage(missile->GetAttackDamage()))
                    Explode();

                missile->Deactivate();
            }
        }
        else if (otherActor->GetName().Contains("Pickup"))
        {
            APickup* pickup = Cast<APickup>(otherActor);
            if(pickup->IsActive())
            {
                HandlePickup(pickup);
                pickup->Deactivate();
            }
        }
    }
}

void AShip::SetPosition(FVector2D position)
{
    RootComponent->SetRelativeLocation(FVector(position.X, m_Depth, position.Y));
}

FVector2D AShip::GetPosition() const
{
    FVector position = RootComponent->GetRelativeLocation();
    return FVector2D(position.X, position.Z);
}

void AShip::SetDepth(float depth)
{
    m_Depth = depth;
    
    // Update RootComponent's position
    FVector position = RootComponent->GetRelativeLocation();
    RootComponent->SetRelativeLocation(FVector(position.X, m_Depth, position.Z));
}

float AShip::GetDepth() const
{
    return m_Depth;
}

void AShip::SetRadians(float radians)
{
    m_Radians = radians;
    RootComponent->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.0f, FMath::RadiansToDegrees(m_Radians), 0.0f)).Rotator());
}

float AShip::GetRadians() const
{
    return m_Radians;
}

FVector2D AShip::Translate(FVector2D displacement)
{
    FVector2D position = GetPosition();
    position += displacement;
    SetPosition(position);
    
    return position;
}

bool AShip::ApplyDamage(int damage)
{
    if (IsInvincible())
        return false;
    
   // Set the player's invincibility timer, since they took damage
   m_InvincibilityTimer = kShipInvincibilityDuration;

   // If the player still has the shield, then take damage from it first
   if (HasShield())
   {
       if (m_Shield->GetHealth() < damage)
       {
           int diff = damage - m_Shield->GetHealth();
           m_Shield->ApplyDamage(m_Shield->GetHealth());
           m_Shield->Deactivate();
           
           damage = diff;
       }
       else
       {
           if (m_Shield->ApplyDamage(damage))
               m_Shield->Deactivate();
           
           if(m_Callback != nullptr)
               m_Callback->OnShipHasTakenDamage(this, damage, m_Health, m_Shield->GetHealth());
           
           return false;
       }
   }
    
    m_Health -= damage;
    
    if(m_Health <= 0)
        m_Health = 0;
    
    if(m_Callback != nullptr)
        m_Callback->OnShipHasTakenDamage(this, damage, m_Health, m_Shield != nullptr ? m_Shield->GetHealth() : 0);
      
    return m_Health == 0;
}

void AShip::SetHealth(int health)
{
    m_Health = health;
}

int AShip::GetHealth() const
{
    return m_Health;
}

void AShip::SetAttackDamage(int attackDamange)
{
    m_AttackDamage = attackDamange;
}

int AShip::GetAttackDamage() const
{
    return m_AttackDamage;
}

void AShip::Activate()
{
    m_IsActive = true;
    
    SetActorHiddenInGame(!m_IsActive);
    SetActorEnableCollision(m_IsActive);
    SetActorTickEnabled(m_IsActive);
}

void AShip::Deactivate()
{
    m_IsActive = false;
    
    SetActorHiddenInGame(!m_IsActive);
    SetActorEnableCollision(m_IsActive);
    SetActorTickEnabled(m_IsActive);
}

bool AShip::IsActive() const
{
    return m_IsActive;
}

FVector2D AShip::GetSpriteSize() const
{
    return m_SpriteSize;
}

void AShip::SetAlpha(float alpha)
{
    FLinearColor spriteColor = m_Sprite->GetSpriteColor();
    spriteColor.A = alpha;
    m_Sprite->SetSpriteColor(spriteColor);
}

float AShip::GetAlpha() const
{
    FLinearColor spriteColor = m_Sprite->GetSpriteColor();
    return spriteColor.A;
}

void AShip::Spawn()
{
    m_IsSpawning = true;
    
    HealUp();

    FVector2D start(kShipOffScreenSpawnX, (kScreenHeight - kHudHeight) * 0.5f);
    FVector2D target(kShipSpawnX, (kScreenHeight - kHudHeight) * 0.5f);
    SetPosition(start);
    SetRadians(0.0f);

    m_SpawnSlider.StartSlide(start, target, 1.0f, this);
}

void AShip::HealUp()
{
    SetHealth(kShipInitialHealth);

    m_Shield->Activate();
    m_Shield->SetHealth(kShipInitialShieldHealth);
}

void AShip::Reset()
{
    Deactivate();
    SetPosition(FVector2D(kShipOffScreenSpawnX, (kScreenHeight - kHudHeight) * 0.5f));
    SetRadians(0.0f);

    m_InvincibilityTimer = 0.0f;
    m_IsSpawning = false;

    m_LinearVelocity = FVector2D::ZeroVector;
    m_Acceleration = FVector2D::ZeroVector;

    HealUp();
}

void AShip::SetCallback(ShipCallback* callback)
{
    m_Callback = callback;
}

bool AShip::HasShield() const
{
    return m_Shield != nullptr && m_Shield->IsActive();
}

AShield* AShip::GetShield() const
{
    return m_Shield;
}

bool AShip::IsInvincible() const
{
    return m_InvincibilityTimer > 0.0f;
}

USpawner* AShip::GetSpawner()
{
    USpawner* spawner = GetGameMode()->GetSpawner();
    check(spawner != nullptr);
    
    return spawner;
}

ASpaceShooterGameModeBase* AShip::GetGameMode()
{
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    check(gameMode != nullptr);
    
    return gameMode;
}

void AShip::FireLaser(bool left)
{
    float radians = GetRadians();
    float magnitude = (m_SpriteSize.Y * 0.5f) - kShipGunOffset;
    float radiansTilt = left ? -kShipGunAngleTilt : kShipGunAngleTilt;
    float edgeRadians = radians + (left ? (PI * 0.5f) : -(PI * 0.5f));
    FVector2D direction(FMath::Cos(radians + radiansTilt), FMath::Sin(radians + radiansTilt));
    FVector2D edge = FVector2D(FMath::Cos(edgeRadians), FMath::Sin(edgeRadians)) * magnitude;
    FVector2D position = GetPosition() + edge;
    FVector2D linearVelocity = direction * kLaserSpeed;

    GetSpawner()->SpawnBlueLaser(position, linearVelocity);
}

void AShip::Explode()
{
    if (IsActive())
    {
        GetSpawner()->SpawnExplosion(GetPosition());

        AShipWing* leftWing = GetSpawner()->SpawnWing(GetPosition(), GetRadians(), "Ship-LeftWing", true);
        if (leftWing)
            leftWing->FadeOut(kShipWingExplosionFadeOutTime);

        AShipWing* rightWing = GetSpawner()->SpawnWing(GetPosition(), GetRadians(), "Ship-RightWing", false);
        if (rightWing)
            rightWing->FadeOut(kShipWingExplosionFadeOutTime);

        Deactivate();
        
        if (m_Callback != nullptr)
            m_Callback->OnShipHasExploded(this);
    }
}

void AShip::HandlePickup(APickup* pickup)
{
    if (pickup->GetPickupType() == APickup::Type::Shield)
    {
        if (!m_Shield->IsActive())
            m_Shield->Activate();

        if (m_Shield->GetHealth() < kShipInitialShieldHealth)
            m_Shield->SetHealth(m_Shield->GetHealth() + 1);
        
        if(m_Callback != nullptr)
            m_Callback->OnShipHasPickedUpShield(this, m_Shield->GetHealth());
    }
}

void AShip::OnFire()
{
    if (!m_IsSpawning && GetHealth() > 0)
    {
        FireLaser(true);
        FireLaser(false);
    }
}

void AShip::OnMovementUpStart()
{
    if (!m_IsSpawning && GetHealth() > 0)
    {
        m_Acceleration.Y += kShipAcceleration;
        m_LinearVelocity.Y = m_Acceleration.Y;
    }
}

void AShip::OnMovementDownStart()
{
    if (!m_IsSpawning && GetHealth() > 0)
    {
        m_Acceleration.Y -= kShipAcceleration;
        m_LinearVelocity.Y = m_Acceleration.Y;
    }
}

void AShip::OnMovementLeftStart()
{
    if (!m_IsSpawning && GetHealth() > 0)
    {
        m_Acceleration.X -= kShipAcceleration;
        m_LinearVelocity.X = m_Acceleration.X;
    }
}

void AShip::OnMovementRightStart()
{
    if (!m_IsSpawning && GetHealth() > 0)
    {
        m_Acceleration.X += kShipAcceleration;
        m_LinearVelocity.X = m_Acceleration.X;
    }
}

void AShip::OnMovementUpStop()
{
    if (!m_IsSpawning && GetHealth() > 0)
    {
        m_Acceleration.Y -= kShipAcceleration;
    }
}

void AShip::OnMovementDownStop()
{
    if (!m_IsSpawning && GetHealth() > 0)
    {
        m_Acceleration.Y += kShipAcceleration;
    }
}

void AShip::OnMovementLeftStop()
{
    if (!m_IsSpawning && GetHealth() > 0)
    {
        m_Acceleration.X += kShipAcceleration;
    }
}

void AShip::OnMovementRightStop()
{
    if (!m_IsSpawning && GetHealth() > 0)
    {
        m_Acceleration.X -= kShipAcceleration;
    }
}

void AShip::OnSlideHasCompleted(class VSlider* slider)
{
    m_IsSpawning = false;
}

