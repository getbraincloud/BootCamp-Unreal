// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.h"
#include "VSlider.h"
#include "BaseActor.h"
#include "Boss.generated.h"

class SPACESHOOTER_API BossCallback
{
public:
    virtual ~BossCallback() = default;
    virtual void OnBossHasExploded(class ABoss* boss) = 0;
};


UCLASS()
class SPACESHOOTER_API ABoss : public ABaseActor, public VSliderCallback
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoss();
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    virtual void NotifyActorBeginOverlap(class AActor* otherActor) override;

    void Spawn();
    void Explode();

    void SetCallback(BossCallback* callback);
    
private:
    void FireMissile(AMissile::Size size);
    void ResetMissileDelay(AMissile::Size size);
    void SetupNextMovement();
    void SetupUpMovement();
    void SetupDownMovement();

    //void OnTriggerEnter2D(Collider2D other);
    void OnSlideHasCompleted(class VSlider* slider) override;

    BossCallback* m_Callback;
    float m_SmallMissileDelay;
    float m_BigMissileDelay;
    float m_MovementDelay;
    VSlider m_Slider;

    enum class State
    {
        Unknown = -1,
        Spawning,
        Attacking,
        Dead
    };

    enum class MovementDirection
    {
        Unknown = -1,
        Up,
        Down
    };

    State m_State;
    MovementDirection m_LastMovementDirection;
};
