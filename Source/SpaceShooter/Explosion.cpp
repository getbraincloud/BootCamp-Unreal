// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "Explosion.h"
#include "Constants.h"
#include "SpaceShooterGameModeBase.h"
#include "PaperFlipbook.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AExplosion::AExplosion() :
    m_Delay(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    m_ExplosionAnimation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ExplosionAnimation"));
    m_ExplosionAnimation->SetSimulatePhysics(false);
    m_ExplosionAnimation->SetConstraintMode(EDOFMode::SixDOF);
    m_ExplosionAnimation->BodyInstance.bLockXRotation;
    m_ExplosionAnimation->BodyInstance.bLockYRotation;
    m_ExplosionAnimation->BodyInstance.bLockZRotation;
    m_ExplosionAnimation->BodyInstance.bLockYTranslation;
    m_ExplosionAnimation->SetLooping(false);
    m_ExplosionAnimation->OnFinishedPlaying.AddDynamic(this, &AExplosion::OnExplosionEnd);
    m_ExplosionAnimation->SetupAttachment(m_Root);

    
    static ConstructorHelpers::FObjectFinder<UPaperFlipbook> flipbookAsset(TEXT("/Game/Sprites/Explosion"));
    if (flipbookAsset.Succeeded())
        m_ExplosionAnimation->SetFlipbook(flipbookAsset.Object);
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
    SetDepth(kExplosionLayer);
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    float timeScale = GetGameMode()->GetTimeScale();
    
    if (m_Delay > 0.0f)
    {
        m_Delay -= DeltaTime * timeScale;

        if (m_Delay < 0.0f)
        {
            m_Delay = 0.0f;
            Explode();
        }
    }
}

void AExplosion::Spawn(FVector2D position, FVector2D scale, float delay)
{
    m_Delay = delay;
    
    SetPosition(position);
    SetScale(scale);

    if (m_Delay == 0.0f)
        Explode();
}

void AExplosion::Explode()
{
    Activate();
    m_ExplosionAnimation->PlayFromStart();
}

void AExplosion::OnExplosionEnd()
{
    Deactivate();
}

