// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "ScrollingBackground.h"
#include "Constants.h"
#include "SpaceShooterGameModeBase.h"


// Sets default values
AScrollingBackground::AScrollingBackground()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_Root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(m_Root);
    
    m_BackgroundSegment1 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundSegment1"));
    m_BackgroundSegment1->SetSimulatePhysics(false);
    m_BackgroundSegment1->SetConstraintMode(EDOFMode::SixDOF);
    m_BackgroundSegment1->BodyInstance.bLockXRotation;
    m_BackgroundSegment1->BodyInstance.bLockYRotation;
    m_BackgroundSegment1->BodyInstance.bLockZRotation;
    m_BackgroundSegment1->BodyInstance.bLockYTranslation;
    m_BackgroundSegment1->SetupAttachment(m_Root);
    
    m_BackgroundSegment2 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundSegment2"));
    m_BackgroundSegment2->SetSimulatePhysics(false);
    m_BackgroundSegment2->SetConstraintMode(EDOFMode::SixDOF);
    m_BackgroundSegment2->BodyInstance.bLockXRotation;
    m_BackgroundSegment2->BodyInstance.bLockYRotation;
    m_BackgroundSegment2->BodyInstance.bLockZRotation;
    m_BackgroundSegment2->BodyInstance.bLockYTranslation;
    m_BackgroundSegment2->SetupAttachment(m_Root);
    m_BackgroundSegment2->SetRelativeLocation(FVector(kScreenWidth, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AScrollingBackground::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScrollingBackground::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    float timeScale = 1.0f;
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    if(gameMode != nullptr)
        timeScale = gameMode->GetTimeScale();
    
    float displacement = kBackgroundSpeed * DeltaTime * timeScale;
    MoveSegment(m_BackgroundSegment1, displacement);
    MoveSegment(m_BackgroundSegment2, displacement);
}

void AScrollingBackground::MoveSegment(UPaperSpriteComponent* backgroundSegment, float displacement)
{
    FVector position = backgroundSegment->GetRelativeLocation();
    
    position.X -= displacement;
    
    if(position.X < -(float)kScreenWidth)
        position.X += (float)kScreenWidth * 2.0f;
    
    backgroundSegment->SetRelativeLocation(position);
}

