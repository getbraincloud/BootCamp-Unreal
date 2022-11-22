// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpaceShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API ASpaceShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
