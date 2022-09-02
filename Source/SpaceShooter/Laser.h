// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.h"
#include "Laser.generated.h"

UCLASS()
class SPACESHOOTER_API ALaser : public ABaseActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaser();
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    enum class Color
    {
        Unknown = -1,
        Blue,
        Red
    };
    
    void Spawn(FVector2D position, FVector2D linearVelocity, Color color);
    
    FVector2D GetFront();
    Color GetLaserColor();
    
private:
    FVector2D m_LinearVelocity;
    Color m_Color;
};
