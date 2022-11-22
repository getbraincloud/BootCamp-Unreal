// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "SpaceShooterGameModeBase.h"
#include "SpaceShooterPlayerController.h"
#include "SpaceShooterHUD.h"
#include "NetworkManager.h"
#include "Ship.h"
#include "Spawner.h"
#include "Constants.h"
#include "EngineUtils.h"
#include "Engine.h"


ASpaceShooterGameModeBase::ASpaceShooterGameModeBase() :
    m_HUD(nullptr),
    m_Ship(nullptr),
    m_Spawner(nullptr),
    m_NetworkManager(nullptr),
    m_LeaderboardsManager(nullptr),
    m_ElapsedTime(0.0f),
    m_TimeScale(1.0f),
    m_LevelIndicatorDisplayTime(0.0f),
    m_EndOfGameDisplayTime(0.0f),
    m_LevelIndex(-1),
    m_LevelCount(0),
    m_GameState(GameModeState::Authenticating),
    m_Mode(Mode::Unknown)
{
    PrimaryActorTick.bCanEverTick = true;
    
    DefaultPawnClass = AShip::StaticClass();
    PlayerControllerClass = ASpaceShooterPlayerController::StaticClass();
    HUDClass = ASpaceShooterHUD::StaticClass();
    
    // Create the Spawner
    m_Spawner = CreateDefaultSubobject<USpawner>("Spawner");
    
    // Create the Network Manager
    m_NetworkManager = CreateDefaultSubobject<UNetworkManager>("NetworkManager");
    m_NetworkManager->SetCallback(this);
    
    // Create the Leaderboards Manager
    m_LeaderboardsManager = CreateDefaultSubobject<ULeaderboardsManager>("LeaderboardsManager");
}

void ASpaceShooterGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetGameViewport()->HandleViewModeCommand(TEXT("unlit"), *GLog, GetWorld());
    GetSpawner()->CreateObjectPools();
    GetDialogManager()->CreateDialogs();
    GetShip()->SetCallback(this);
    
    HandleAuthentication();
}

void ASpaceShooterGameModeBase::Tick(float DeltaTime)
{
    if(m_NetworkManager != nullptr)
        m_NetworkManager->Tick(DeltaTime);
    
    if(m_Spawner != nullptr)
        m_Spawner->Tick(DeltaTime);
    
    if (m_GameState == GameModeState::GameOver)
    {
        if (m_EndOfGameDisplayTime > 0.0f)
        {
            m_EndOfGameDisplayTime -= DeltaTime;
            if (m_EndOfGameDisplayTime < 0.0f)
            {
                if (m_GameState == GameModeState::GameOver)
                    m_HUD->HideGameOverHUD();
                if (m_GameState == GameModeState::Victory)
                    m_HUD->HideGameWonHUD();

                if (m_Mode == Mode::Endless)
                {
                    if (GetNetworkManager()->IsUsernameSaved())
                        GetNetworkManager()->PostScoreToLeaderboard(kBrainCloudMainLeaderboardID, m_ElapsedTime);
                    else
                        GetDialogManager()->ShowPostScoreDialog(m_ElapsedTime);
                }
                else if (m_Mode == Mode::Horde)
                {
                    GetDialogManager()->ShowPlayAgainDialog();
                }
            }
        }
    }
    else if (m_GameState == GameModeState::Gameplay)
    {
        m_ElapsedTime += DeltaTime * m_TimeScale;
        m_HUD->SetElapsedTimeHUD(m_ElapsedTime);

        if (m_Mode == Mode::Endless)
        {
            //TODO: Implement Highscores
        }
        else if (m_Mode == Mode::Horde)
        {
            if (m_ElapsedTime > m_LevelDurations[m_LevelIndex] && m_LevelDurations[m_LevelIndex] != -1.0f)
            {
                m_ElapsedTime = m_LevelDurations[m_LevelIndex];
                NextLevel();
            }
        }
    }
    else if (m_GameState == GameModeState::LevelTransition)
    {
        if (m_LevelIndicatorDisplayTime > 0.0f)
        {
            m_LevelIndicatorDisplayTime -= DeltaTime;

            if (m_LevelIndicatorDisplayTime < 0.0f)
                StartLevel();
        }
    }
}

void ASpaceShooterGameModeBase::HandleAuthentication()
{
    m_GameState = GameModeState::Authenticating;
    
    if (GetNetworkManager()->HasAuthenticatedPreviously())
    {
        GetDialogManager()->ShowConnectingDialog();
        GetNetworkManager()->Reconnect();
    }
    else
    {
        GetDialogManager()->ShowEmailLoginDialog();
    }
}

bool ASpaceShooterGameModeBase::IsGameOver()
{
    return m_GameState == GameModeState::GameOver;
}

bool ASpaceShooterGameModeBase::IsGameWon()
{
    return m_GameState == GameModeState::Victory;
}

AShip* ASpaceShooterGameModeBase::GetShip() 
{
    if(m_Ship == nullptr)
    {
        m_Ship = Cast<AShip>(GetWorld()->GetFirstPlayerController()->GetPawn());
        check(m_Ship != nullptr);
    }
    
    return m_Ship;
}

ASpaceShooterHUD* ASpaceShooterGameModeBase::GetHUD()
{
    if(m_HUD == nullptr)
    {
        m_HUD = Cast<ASpaceShooterHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
        check(m_HUD != nullptr);
    }
    
    return m_HUD;
}

UDialogManager* ASpaceShooterGameModeBase::GetDialogManager()
{
    return GetHUD()->GetDialogManager();
}

USpawner* ASpaceShooterGameModeBase::GetSpawner()
{
    return m_Spawner;
}

UNetworkManager* ASpaceShooterGameModeBase::GetNetworkManager() const
{
    return m_NetworkManager;
}

ULeaderboardsManager* ASpaceShooterGameModeBase::GetLeaderboardsManager() const
{
    return m_LeaderboardsManager;
}

void ASpaceShooterGameModeBase::StartEndlessMode()
{
    m_Mode = Mode::Endless;
    PrepareLevel(-1); // -1 is the default level used for endless mode
}

void ASpaceShooterGameModeBase::StartHordeMode(int levelIndex)
{
    m_Mode = Mode::Horde;
    PrepareLevel(levelIndex);
}

void ASpaceShooterGameModeBase::ResetGame(bool startNewGame)
{
    GetSpawner()->Reset();
    GetShip()->Reset();
    GetHUD()->ResetHUD();

    if (startNewGame)
    {
        if (m_Mode == Mode::Endless)
            StartEndlessMode();
        else if (m_Mode == Mode::Horde)
            StartHordeMode();
    }
}

void ASpaceShooterGameModeBase::SetTimeScale(float timeScale)
{
    m_TimeScale = timeScale;
}

float ASpaceShooterGameModeBase::GetTimeScale()
{
    return m_TimeScale;
}

void ASpaceShooterGameModeBase::OnCheatAddTime()
{
    m_ElapsedTime += 15.0f;
}

void ASpaceShooterGameModeBase::OnCheatNextLevel()
{
    if(m_Mode == Mode::Horde)
        NextLevel();
}

void ASpaceShooterGameModeBase::OnCheatHealUp()
{
    GetShip()->HealUp();
}

void ASpaceShooterGameModeBase::PrepareLevel(int levelIndex)
{   
    m_GameState = GameModeState::LevelTransition;

    m_LevelIndicatorDisplayTime = kLevelDisplayDuration;
    m_LevelIndex = levelIndex;

    m_ElapsedTime = 0.0f;

    if (m_Mode == Mode::Endless)
    {
        //TODO: Implement Highscores

        m_HUD->ShowLevelHUD(-1, "");
    }
    else if (m_Mode == Mode::Horde)
    {
        GetHUD()->ShowLevelHUD(m_LevelIndex + 1, m_LevelDescriptions[m_LevelIndex]);
        GetHUD()->PushLevelGoal(m_LevelDescriptions[m_LevelIndex]);
    }
}

void ASpaceShooterGameModeBase::StartLevel()
{
    m_GameState = GameModeState::Gameplay;

    m_HUD->HideLevelHUD();

    GetSpawner()->StartSpawning(m_LevelIndex);

    if (!GetShip()->IsActive())
    {
        GetShip()->Activate();
        GetShip()->Spawn();
    }
}

void ASpaceShooterGameModeBase::NextLevel()
{
    GetSpawner()->StopSpawning();
    GetSpawner()->ExplodeAllActive();

    if (m_LevelIndex + 1 < m_LevelCount)
    {
        GetShip()->HealUp();
        m_HUD->UpdateShipDamageHUD(1.0f, 1.0f);

        PrepareLevel(m_LevelIndex + 1);
    }
}

void ASpaceShooterGameModeBase::OnShipHasTakenDamage(AShip* ship, int damageTaken, int currentHealth, int currentShield)
{
    float healthPct = (float)currentHealth / (float)kShipInitialHealth;
    float shieldPct = (float)currentShield / (float)kShipInitialShieldHealth;
    m_HUD->UpdateShipDamageHUD(healthPct, shieldPct);
}

void ASpaceShooterGameModeBase::OnShipHasPickedUpShield(class AShip* ship, int currentShield)
{
    float healthPct = (float)ship->GetHealth() / (float)kShipInitialHealth;
    float shieldPct = (float)currentShield / (float)kShipInitialShieldHealth;
    m_HUD->UpdateShipDamageHUD(healthPct, shieldPct);
}

void ASpaceShooterGameModeBase::OnShipHasExploded(AShip* ship)
{
    m_GameState = GameModeState::GameOver;
    m_EndOfGameDisplayTime = kEndOfGameDisplayDuration;
    m_HUD->ShowGameOverHUD();
}

void ASpaceShooterGameModeBase::OnBossHasExploded(ABoss* boss)
{
    m_GameState = GameModeState::Victory;
    m_EndOfGameDisplayTime = kEndOfGameDisplayDuration;
    m_HUD->ShowGameWonHUD();

    GetSpawner()->StopSpawning();
    GetSpawner()->ExplodeAllActive();
}

void ASpaceShooterGameModeBase::OnAuthenticationRequestCompleted()
{
    if (m_GameState == GameModeState::Authenticating)
    {
        m_GameState = GameModeState::LoadingData;
        
        GetNetworkManager()->RequestLeaderboard(kBrainCloudMainLeaderboardID);
    }
}

void ASpaceShooterGameModeBase::OnLogOutRequestCompleted()
{
    UBrainCloudDialogWidget* brainCloudDialog = Cast<UBrainCloudDialogWidget>(GetDialogManager()->GetDialogByType(UDialogWidget::Type::BrainCloud));
    brainCloudDialog->Refresh();
}

void ASpaceShooterGameModeBase::OnLeaderboardRequestCompleted(const Leaderboard& leaderboard)
{
    GetLeaderboardsManager()->AddLeaderboard(leaderboard);
    
    if (IsGameOver())
    {
        if (m_Mode == Mode::Endless)
        {
            GetDialogManager()->ShowPlayAgainDialog();
            GetDialogManager()->ShowLeaderboardsDialog();
        }
    }
    else
    {
        GetDialogManager()->HideConnectingDialog();
        GetDialogManager()->ShowMainMenuDialog();
    }
}

void ASpaceShooterGameModeBase::OnPostScoreToLeaderboardRequestCompleted()
{
    GetLeaderboardsManager()->SetUserTime(m_ElapsedTime);
    GetNetworkManager()->RequestLeaderboard(kBrainCloudMainLeaderboardID);
}
