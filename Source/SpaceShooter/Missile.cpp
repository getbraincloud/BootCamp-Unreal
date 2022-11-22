// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "Missile.h"
#include "Constants.h"
#include "SpaceShooterGameModeBase.h"
#include "PaperSprite.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AMissile::AMissile() :
    m_LinearVelocity(FVector2D::ZeroVector),
    m_Size(Size::Unknown)
{
 	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UPaperSprite> spriteAsset(TEXT("/Game/Sprites/Missile-Small_Sprite"));
    if (spriteAsset.Succeeded())
        m_Sprite->SetSprite(spriteAsset.Object);
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
    SetDepth(kMissileLayer);
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    float timeScale = GetGameMode()->GetTimeScale();
    FVector2D position = Translate(m_LinearVelocity * DeltaTime * timeScale);

    if (position.X > kScreenWidth || position.X < -GetSpriteSize().X)
        Deactivate();
}


void AMissile::Spawn(FVector2D position, FVector2D linearVelocity, Size size)
{
    if (size != Size::Unknown)
    {
        SetPosition(position);
        SetRadians(FMath::Atan2(linearVelocity.Y, linearVelocity.X));

        m_LinearVelocity = linearVelocity;
        m_Size = size;

        FString texture = m_Size == Size::Small ? "Missile-Small" : "Missile-Big";
        SetSprite(texture);

        if(m_Size == Size::Small)
            SetAttackDamage(kMissileSmallAttackDamage);
        else
            SetAttackDamage(kMissileBigAttackDamage);

        
        Activate();
    }
}

FVector2D AMissile::GetFront()
{
    FVector2D position = GetPosition();
    FVector2D normal = m_LinearVelocity;
    normal.Normalize();
    
    FVector2D displacement = normal * GetSpriteSize().X;
    return position + displacement;
}

FVector2D AMissile::GetMiddle()
{
    FVector2D position = GetPosition();
    FVector2D normal = m_LinearVelocity;
    normal.Normalize();
    
    FVector2D displacement = normal * (GetSpriteSize().X * 0.5f);
    return position + displacement;
}

AMissile::Size AMissile::GetMissleSize()
{
    return m_Size;
}
