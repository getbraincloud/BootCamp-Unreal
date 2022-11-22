// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.h"
#include "Pickup.generated.h"

UCLASS()
class SPACESHOOTER_API APickup : public ABaseActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    enum class Type
    {
        Unknown = -1,
        Shield
    };
    
    void Spawn(FVector2D position, FVector2D linearVelocity, Type type);
    Type GetPickupType() const;
    
private:
    FVector2D m_LinearVelocity;
    float m_Lifetime;
    Type m_Type;
};
