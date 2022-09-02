// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "ShipWing.h"
#include "Constants.h"
#include "SpaceShooterGameModeBase.h"
#include "PaperSprite.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AShipWing::AShipWing() :
    m_LinearVelocity(FVector2D::ZeroVector),
    m_AngularVelocity(0.0f),
    m_SpinDirection(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UPaperSprite> spriteAsset(TEXT("/Game/Sprites/AsteroidBig-1_Sprite"));
    if (spriteAsset.Succeeded())
        m_Sprite->SetSprite(spriteAsset.Object);
}

// Called when the game starts or when spawned
void AShipWing::BeginPlay()
{
	Super::BeginPlay();
    SetDepth(kShipWingLayer);
}

// Called every frame
void AShipWing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    float timeScale = GetGameMode()->GetTimeScale();

    Translate(m_LinearVelocity * DeltaTime * timeScale);

    float radians = GetRadians();
    radians += m_AngularVelocity * m_SpinDirection * DeltaTime * timeScale;
    if (m_SpinDirection > 0.0f && radians > PI * 2.0f)
    {
        radians -= PI * 2.0f;
    }
    else if (m_SpinDirection < 0.0f && radians < -(PI * 2.0f))
    {
        radians += PI * 2.0f;
    }
    
    SetRadians(radians);
}

void AShipWing::Spawn(FVector2D position, FVector2D linearVelocity, float rotation, const FString& texture)
{
    ResetFade();
    
    SetPosition(position);
    SetRadians(rotation);
    SetSprite(texture);
    Activate();

    m_LinearVelocity = linearVelocity;
    m_AngularVelocity = FMath::RandRange(kShipWingMinAngularVelocity, kShipWingMaxAngularVelocity);
    m_SpinDirection = FMath::RandRange(0, 1) == 1 ? 1.0f : -1.0f;
}
