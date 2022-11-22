// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Laser.h"
#include "BaseActor.h"
#include "GameFramework/Actor.h"
#include "LaserImpact.generated.h"

UCLASS()
class SPACESHOOTER_API ALaserImpact : public ABaseActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaserImpact();
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void Spawn(FVector2D position, ALaser::Color color);

private:
    float m_Lifetime;
    ALaser::Color m_Color;
    
};
