// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "Boss.h"
#include "Spawner.h"
#include "NetworkManager.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceShooterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API ASpaceShooterGameModeBase : public AGameModeBase, public ShipCallback, public BossCallback
{
	GENERATED_BODY()
	
public:
    ASpaceShooterGameModeBase();
    
protected:
    virtual void BeginPlay() override;
    
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
    bool IsGameOver();
    bool IsGameWon();
    
    AShip* GetShip();
    
    class ASpaceShooterHUD* GetHUD();
    class UDialogManager* GetDialogManager();
    
    USpawner* GetSpawner();
    UNetworkManager* GetNetworkManager() const;
    
    void StartEndlessMode();
    void StartHordeMode(int levelIndex = 0);
    void ResetGame(bool startNewGame);
    
    void SetTimeScale(float timeScale);
    float GetTimeScale();
    
    void OnCheatAddTime();
    void OnCheatNextLevel();
    void OnCheatHealUp();

private:
    void PrepareLevel(int levelIndex);
    void StartLevel();
    void NextLevel();
    
    void OnShipHasTakenDamage(class AShip* ship, int damageTaken, int currentHealth, int currentShield) override;
    void OnShipHasPickedUpShield(class AShip* ship, int currentShield) override;

    void OnShipHasExploded(AShip* ship) override;
    void OnBossHasExploded(ABoss* boss) override;

    enum class GameModeState
    {
        Authenticating,
        LoadingData,
        Gameplay,
        LevelTransition,
        GameOver,
        Victory
    };
    
    enum class Mode
    {
        Unknown = -1,
        Endless,
        Horde
    };
    
    class ASpaceShooterHUD* m_HUD;
    AShip* m_Ship;
    UPROPERTY() USpawner* m_Spawner;
    UPROPERTY() UNetworkManager* m_NetworkManager;
    std::vector<float> m_LevelDurations;
    std::vector<FString> m_LevelDescriptions;
    float m_ElapsedTime;
    float m_TimeScale;
    float m_LevelIndicatorDisplayTime;
    float m_EndOfGameDisplayTime;
    int m_LevelIndex;
    int m_LevelCount;
    GameModeState m_GameState;
    Mode m_Mode;
};
