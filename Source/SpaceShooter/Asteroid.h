// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.h"
#include "Asteroid.generated.h"

UCLASS()
class SPACESHOOTER_API AAsteroid : public ABaseActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroid();
            
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    virtual void NotifyActorBeginOverlap(class AActor* otherActor) override;

    enum class Size
    {
        Unknown = -1,
        Big,
        Medium,
        Small,
        Tiny
    };
    
    Size GetSize();
    
    void Spawn(FVector2D position, FVector2D linearVelocity, Size size);
    void Explode(bool onlySmallDebris = false);

private:
    FVector2D m_LinearVelocity;
    float m_AngularVelocity;
    float m_SpinDirection;
    Size m_Size;
};
