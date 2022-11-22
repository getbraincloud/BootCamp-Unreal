// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "Pickup.h"
#include "Constants.h"
#include "SpaceShooterGameModeBase.h"
#include "PaperSprite.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
APickup::APickup() :
    m_LinearVelocity(FVector2D::ZeroVector),
    m_Lifetime(0.0f),
    m_Type(Type::Unknown)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    static ConstructorHelpers::FObjectFinder<UPaperSprite> spriteAsset(TEXT("/Game/Sprites/ShieldPowerup_Sprite"));
    if (spriteAsset.Succeeded())
        m_Sprite->SetSprite(spriteAsset.Object);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
    SetDepth(kPickupLayer);
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    float timeScale = GetGameMode()->GetTimeScale();

    if (m_Lifetime > 0.0f)
    {
        m_Lifetime -= DeltaTime * timeScale;
        if (m_Lifetime <= 0.0f)
        {
            m_Lifetime = 0.0f;
            FadeOut(kPickupFadeOutTime);
        }
    }

    FVector2D position = Translate(m_LinearVelocity * DeltaTime * timeScale);

    if (position.X < -GetSpriteSize().X)
        Deactivate();
}

void APickup::Spawn(FVector2D position, FVector2D linearVelocity, Type type)
{
    if(type != Type::Unknown)
    {
        SetPosition(position);
        
        m_Type = type;
        m_LinearVelocity = linearVelocity;
        m_Lifetime = kPickupLifetime;
        
        Activate();
    }
}

APickup::Type APickup::GetPickupType() const
{
    return m_Type;
}
