// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.h"
#include "Missile.generated.h"

UCLASS()
class SPACESHOOTER_API AMissile : public ABaseActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissile();
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    enum class Size
    {
        Unknown = -1,
        Small,
        Big
    };
    
    void Spawn(FVector2D position, FVector2D linearVelocity, Size size);

    FVector2D GetFront();
    FVector2D GetMiddle();
    Size GetMissleSize();
    
private:   
    FVector2D m_LinearVelocity;
    Size m_Size;
};
