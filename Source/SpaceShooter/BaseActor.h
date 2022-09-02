// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Fader.h"
#include "BaseActor.generated.h"


UCLASS()
class SPACESHOOTER_API ABaseActor : public AActor, public FaderCallback
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseActor();

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Components) USceneComponent* m_Root;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Actor Visual") UPaperSpriteComponent* m_Sprite;
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    
    //virtual void NotifyActorBeginOverlap(class AActor * OtherActor) override;

    void SetPosition(FVector2D position);
    FVector2D GetPosition() const;
    
    void SetScale(FVector2D scale);
    FVector2D GetScale() const;
        
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
    
    void FadeOut(float duration, float delay = 0.0f);
    bool IsFadingOut() const;
    
    void FadeIn(float duration, float delay = 0.0f);
    bool IsFadingIn() const;
    
    float GetTimeScale();
    
protected:
    void SetSprite(const FString& texture);

    class ASpaceShooterGameModeBase* GetGameMode();
    class USpawner* GetSpawner();
    
    void ResetFade();
    void OnFadeOutHasCompleted(Fader* fader) override;
    
private:
    Fader m_Fader;
    FVector2D m_SpriteSize;
    float m_Depth;
    float m_Radians;
    int m_Health;
    int m_AttackDamage;
    bool m_IsActive;
};
