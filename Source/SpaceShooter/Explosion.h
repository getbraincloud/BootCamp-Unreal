// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "BaseActor.h"
#include "Explosion.generated.h"

UCLASS()
class SPACESHOOTER_API AExplosion : public ABaseActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosion();
        
    UPROPERTY(VisibleAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true")) UPaperFlipbookComponent* m_ExplosionAnimation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void Spawn(FVector2D position, FVector2D scale, float delay);
    
private:
    void Explode();
    
    UFUNCTION(Category = "CallBack") void OnExplosionEnd();

    float m_Delay;
};
