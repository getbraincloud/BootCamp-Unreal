// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "LaserImpact.h"
#include "Constants.h"
#include "SpaceShooterGameModeBase.h"
#include "PaperSprite.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
ALaserImpact::ALaserImpact() :
    m_Lifetime(0.0f),
    m_Color(ALaser::Color::Unknown)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UPaperSprite> spriteAsset(TEXT("/Game/Sprites/LaserImpact-Blue_Sprite"));
    if (spriteAsset.Succeeded())
        m_Sprite->SetSprite(spriteAsset.Object);
}

// Called when the game starts or when spawned
void ALaserImpact::BeginPlay()
{
	Super::BeginPlay();
    SetDepth(kLaserImpactLayer);
}

// Called every frame
void ALaserImpact::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    float timeScale = GetGameMode()->GetTimeScale();
    
    if (m_Lifetime > 0.0f)
    {
        m_Lifetime -= DeltaTime * timeScale;

        if (m_Lifetime <= 0.0f)
        {
            m_Lifetime = 0.0f;
            Deactivate();
        }
    }
}

void ALaserImpact::Spawn(FVector2D position, ALaser::Color color)
{    
    if (color != ALaser::Color::Unknown)
    {
        SetPosition(position);

        m_Color = color;
        m_Lifetime = kLaserImpactLifetime;

        FString texture = m_Color == ALaser::Color::Blue ? "LaserImpact-Blue" : "LaserImpact-Red";
        SetSprite(texture);

        Activate();
    }
}
