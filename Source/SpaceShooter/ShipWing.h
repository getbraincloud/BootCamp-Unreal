// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.h"
#include "ShipWing.generated.h"

UCLASS()
class SPACESHOOTER_API AShipWing : public ABaseActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShipWing();
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void Spawn(FVector2D position, FVector2D linearVelocity, float rotation, const FString& texture);
    
private:
    FVector2D m_LinearVelocity;
    float m_AngularVelocity;
    float m_SpinDirection;
};
