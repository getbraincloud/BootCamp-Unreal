// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "Laser.h"
#include "Constants.h"
#include "SpaceShooterGameModeBase.h"
#include "PaperSprite.h"
#include "Math/UnrealMathUtility.h"
#include <string>


ALaser::ALaser() :
    m_LinearVelocity(FVector2D::ZeroVector),
    m_Color(Color::Unknown)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UPaperSprite> spriteAsset(TEXT("/Game/Sprites/Laser-Blue_Sprite"));
    if (spriteAsset.Succeeded())
        m_Sprite->SetSprite(spriteAsset.Object);
}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
    SetDepth(kLaserLayer);
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    float timeScale = GetGameMode()->GetTimeScale();

    FVector2D position = Translate(m_LinearVelocity * DeltaTime * timeScale);

    if (position.X > kScreenWidth || position.X < -GetSpriteSize().X)
        Deactivate();
}

void ALaser::Spawn(FVector2D position, FVector2D linearVelocity, Color color)
{
    if (color != Color::Unknown)
    {
        SetPosition(position);
        SetRadians(FMath::Atan2(linearVelocity.Y, linearVelocity.X));

        m_LinearVelocity = linearVelocity;
        m_Color = color;

        FString texture = m_Color == Color::Blue ? "Laser-Blue" : "Laser-Red";
        SetSprite(texture);

        Activate();
    }
}

FVector2D ALaser::GetFront()
{
    FVector2D position = GetPosition();
    FVector2D normal = m_LinearVelocity;
    normal.Normalize();
    
    FVector2D displacement = normal * (GetSpriteSize().X * 0.5f);
    return position + displacement;
}

ALaser::Color ALaser::GetLaserColor()
{
    return m_Color;
}

