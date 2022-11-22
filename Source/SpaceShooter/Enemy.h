// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.h"
#include "Enemy.generated.h"

UCLASS()
class SPACESHOOTER_API AEnemy : public ABaseActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    virtual void NotifyActorBeginOverlap(class AActor* otherActor) override;
    
    enum class Type
    {
        Unknown = -1,
        One,
        Two,
        Three,
        Four,
        Five
    };
    
    void Spawn(FVector2D position, FVector2D linearVelocity, Type type, int health);
    void Explode();

private:
    void FireMissle();
    void FireLaser();
    void ResetLaserDelay();
    
    FVector2D m_LinearVelocity;
    Type m_Type;
    float m_LaserDelay;
    float m_EnemyFourFiringDuration;   //Enemy.Type.Four only
    float m_EnemyFourFiringCooldown;   //Enemy.Type.Four only
    float m_EnemyFiveMissileDelay;     //Enemy.Type.Five only
    float m_EnemyFiveMissileCooldown;  //Enemy.Type.Five only
    bool m_LaserAlternate;
};
