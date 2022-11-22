// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PaperSpriteComponent.h"
#include "VSlider.h"
#include "Ship.generated.h"

class SPACESHOOTER_API ShipCallback
{
public:
    virtual ~ShipCallback() = default;
    
    virtual void OnShipHasTakenDamage(class AShip* ship, int damageTaken, int currentHealth, int currentShield) = 0;
    virtual void OnShipHasPickedUpShield(class AShip* ship, int currentShield);
    virtual void OnShipHasExploded(class AShip* ship) = 0;
};


UCLASS()
class SPACESHOOTER_API AShip : public APawn, public VSliderCallback
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip();
    
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components) USceneComponent* m_Root;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Actor Visual") UPaperSpriteComponent* m_Sprite;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void NotifyActorBeginOverlap(class AActor* otherActor) override;
    
    void SetPosition(FVector2D position);
    FVector2D GetPosition() const;
        
    void SetDepth(float depth);
    float GetDepth() const;
    
    void SetRadians(float radians);     // In radians
    float GetRadians() const;           //In radians
    
    FVector2D Translate(FVector2D displacement);
    
    virtual bool ApplyDamage(int damage); //Returns true if damage is enough to kill the object
    void SetHealth(int health);
    int GetHealth() const;
    
    void SetAttackDamage(int attackDamange);
    int GetAttackDamage() const;
    
    void Activate();
    void Deactivate();
    bool IsActive() const;
    
    FVector2D GetSpriteSize() const;
    
    void SetAlpha(float alpha);
    float GetAlpha() const;
    
    void Spawn();
    void HealUp();
    void Reset();
    
    void SetCallback(ShipCallback* callback);

    bool HasShield() const;
    class AShield* GetShield() const;
    
    bool IsInvincible() const;

private:
    class USpawner* GetSpawner();
    class ASpaceShooterGameModeBase* GetGameMode();
    
    void Explode();
    void FireLaser(bool left);
    void HandlePickup(class APickup* pickup);
    
    void OnFire();
    void OnMovementUpStart();
    void OnMovementDownStart();
    void OnMovementLeftStart();
    void OnMovementRightStart();
    void OnMovementUpStop();
    void OnMovementDownStop();
    void OnMovementLeftStop();
    void OnMovementRightStop();
    
    void OnSlideHasCompleted(class VSlider* slider) override;
        
    ShipCallback* m_Callback;
    class AShield* m_Shield;
    FVector2D m_Acceleration;
    FVector2D m_LinearVelocity;
    FVector2D m_SpriteSize;
    float m_InvincibilityTimer;
    float m_Depth;
    float m_Radians;
    int m_Health;
    int m_AttackDamage;
    VSlider m_SpawnSlider;
    bool m_IsSpawning;
    bool m_IsActive;
};
