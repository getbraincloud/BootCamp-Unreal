// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "Shield.h"
#include "Constants.h"
#include "PaperSprite.h"


// Sets default values
AShield::AShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UPaperSprite> spriteAsset(TEXT("/Game/Sprites/Shield_Sprite"));
    if (spriteAsset.Succeeded())
        m_Sprite->SetSprite(spriteAsset.Object);
}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
    SetHealth(kShipInitialShieldHealth);
}
