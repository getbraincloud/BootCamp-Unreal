// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "SpaceShooterHUD.h"
#include "HUDUserWidget.h"
#include "MainMenuDialogWidget.h"
#include "ConnectingDialogWidget.h"
#include "BrainCloudDialogWidget.h"
#include "SpaceShooterGameModeBase.h"
#include "Ship.h"
#include "Shield.h"


ASpaceShooterHUD::ASpaceShooterHUD() :
    m_DialogManager(nullptr),
    m_HUDWidget(nullptr),
    m_DisplayedInfoScroller(nullptr),
    m_LeavingInfoScroller(nullptr),
    m_PlayerHasAllTimeHighScore(false)
{
    // Create the Dialog Manager
    m_DialogManager = CreateDefaultSubobject<UDialogManager>("DialogManager");
}

void ASpaceShooterHUD::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Warning, TEXT("ASpaceShooterHUD::BeginPlay()"));

    // Create the HUD Widget
    m_HUDWidget = CreateWidgetAndValidate<UHUDUserWidget>("HUDWidget");
    
    if(m_HUDWidget != nullptr)
    {
        m_DisplayedInfoScroller = m_HUDWidget->GetHUDInfoScrollerWidget0();
        m_DisplayedInfoScroller->SetCallback(this);
        m_DisplayedInfoScroller->Clear();

        m_LeavingInfoScroller = m_HUDWidget->GetHUDInfoScrollerWidget1();
        m_LeavingInfoScroller->SetCallback(this);
        m_LeavingInfoScroller->Clear();
    }
}

void ASpaceShooterHUD::SetElapsedTimeHUD(float elapsedTime)
{
    if(m_HUDWidget != nullptr)
        m_HUDWidget->SetElapsedTime(elapsedTime);
}

void ASpaceShooterHUD::UpdateShipDamageHUD(float healthPCT, float shieldPCT)
{
    if(m_HUDWidget != nullptr)
    {
        m_HUDWidget->SetHealth(healthPCT);
        m_HUDWidget->SetShield(shieldPCT);
    }
}

void ASpaceShooterHUD::ShowGameOverHUD()
{
    UE_LOG(LogTemp, Warning, TEXT("ASpaceShooterHUD::ShowGameOverHUD()"));
    
    if(m_HUDWidget != nullptr)
        m_HUDWidget->ShowGameOver();
}

void ASpaceShooterHUD::HideGameOverHUD()
{
    UE_LOG(LogTemp, Warning, TEXT("ASpaceShooterHUD::HideGameOverHUD()"));

    if(m_HUDWidget != nullptr)
        m_HUDWidget->HideGameOver();
}

void ASpaceShooterHUD::ShowGameWonHUD()
{
    if(m_HUDWidget != nullptr)
        m_HUDWidget->ShowGameWon();
}

void ASpaceShooterHUD::HideGameWonHUD()
{
    if(m_HUDWidget != nullptr)
        m_HUDWidget->HideGameWon();
}

void ASpaceShooterHUD::ShowLevelHUD(int number, const FString& information)
{
    if(m_HUDWidget != nullptr)
        m_HUDWidget->ShowLevel(number, information);
}

void ASpaceShooterHUD::HideLevelHUD()
{
    if(m_HUDWidget != nullptr)
        m_HUDWidget->HideLevel();
}

void ASpaceShooterHUD::PushAchievement(Achievement* achievement)
{
    if(achievement != nullptr)
    {
        HUDInfoScrollerData infoScrollerData;
        bool hasData = false;
        
        if(m_InfoScrollerData.size() > 0)
        {
            if(m_InfoScrollerData.back().type == HUDInfoScrollerData::Type::LeaderboardEntry || (m_InfoScrollerData.back().type == HUDInfoScrollerData::Type::Text && m_InfoScrollerData.back().text == "NEW #1 HIGH SCORE!"))
            {
                infoScrollerData = m_InfoScrollerData.back();
                hasData = true;
            }
        }
        
        PushInfoScrollerData(HUDInfoScrollerData(achievement, GetInfoScrollerEnteringPosition()));
        
        if(hasData)
            PushInfoScrollerData(infoScrollerData);
    }
}

void ASpaceShooterHUD::PushLeaderboardEntry(LeaderboardEntry* leaderboardEntry)
{
    if(leaderboardEntry != nullptr)
        PushInfoScrollerData(HUDInfoScrollerData(leaderboardEntry, GetInfoScrollerEnteringPosition()));
}

void ASpaceShooterHUD::PushLevelGoal(const FString& levelGoal)
{
    PushInfoScrollerData(HUDInfoScrollerData(levelGoal, GetInfoScrollerEnteringPosition()));
}

void ASpaceShooterHUD::PushPlayerHasAllTimeHighScore()
{
    if (!m_PlayerHasAllTimeHighScore)
    {
        m_PlayerHasAllTimeHighScore = true;
        
        if(m_HUDWidget != nullptr)
            m_HUDWidget->SetElapsedTimeColor(FColor(207, 198, 0, 255));
        
        PushInfoScrollerData(HUDInfoScrollerData("NEW #1 HIGH SCORE!", GetInfoScrollerEnteringPosition()));
    }
}

void ASpaceShooterHUD::ResetHUD()
{
    if(m_HUDWidget != nullptr)
    {
        m_HUDWidget->Reset();
        
        if(m_DisplayedInfoScroller != nullptr)
        {
            m_DisplayedInfoScroller = m_HUDWidget->GetHUDInfoScrollerWidget0();
            m_DisplayedInfoScroller->Clear();
        }

        if(m_LeavingInfoScroller != nullptr)
        {
            m_LeavingInfoScroller = m_HUDWidget->GetHUDInfoScrollerWidget1();
            m_LeavingInfoScroller->Clear();
        }
    }
    
    m_InfoScrollerData.clear();

    m_PlayerHasAllTimeHighScore = false;
}

UDialogManager* ASpaceShooterHUD::GetDialogManager() const
{
    return m_DialogManager;
}

void ASpaceShooterHUD::OnHUDInfoScrollerMoveHasCompleted(UHUDInfoScrollerUserWidget* infoScroller)
{
    if(infoScroller == m_LeavingInfoScroller)
    {
        m_LeavingInfoScroller->Clear();
        m_InfoScrollerData.pop_front();
        
        TryPushingNextInfoScroller();
    }
}

void ASpaceShooterHUD::OnHUDInfoScrollerHoldCompleted(UHUDInfoScrollerUserWidget* infoScroller)
{
    TryPushingNextInfoScroller();
}

void ASpaceShooterHUD::PushInfoScrollerData(const HUDInfoScrollerData& infoScrollerData)
{
    m_InfoScrollerData.emplace_back(infoScrollerData);
    
    if (m_InfoScrollerData.size() == 1)  //If it's the first highscore push it
    {
        m_DisplayedInfoScroller->PushOn(m_InfoScrollerData.front(), GetInfoScrollerPosition());
    }
    else
    {
        TryPushingNextInfoScroller();
    }
}

void ASpaceShooterHUD::TryPushingNextInfoScroller()
{
    if (m_InfoScrollerData.size() >= 2)
    {
        if (!m_DisplayedInfoScroller->IsMoving() && m_DisplayedInfoScroller->CanPush())
        {
            SwapInfoScrollers();
            m_DisplayedInfoScroller->PushOn(*std::next(m_InfoScrollerData.begin()), GetInfoScrollerPosition());
            m_LeavingInfoScroller->PushOff(GetInfoScrollerLeavingPosition());
        }
    }
}

void ASpaceShooterHUD::SwapInfoScrollers()
{
    if(m_DisplayedInfoScroller == m_HUDWidget->GetHUDInfoScrollerWidget0())
    {
        m_DisplayedInfoScroller = m_HUDWidget->GetHUDInfoScrollerWidget1();
        m_LeavingInfoScroller = m_HUDWidget->GetHUDInfoScrollerWidget0();
    }
    else
    {
        m_DisplayedInfoScroller = m_HUDWidget->GetHUDInfoScrollerWidget0();
        m_LeavingInfoScroller = m_HUDWidget->GetHUDInfoScrollerWidget1();
    }
}

FVector2D ASpaceShooterHUD::GetInfoScrollerPosition()
{
    return FVector2D::ZeroVector;
}

FVector2D ASpaceShooterHUD::GetInfoScrollerEnteringPosition()
{
    float x = GetInfoScrollerPosition().X;
    float y = GetInfoScrollerPosition().Y + kHudHeight + kHudSmallBuffer;
    return FVector2D(x, y);
}

FVector2D ASpaceShooterHUD::GetInfoScrollerLeavingPosition()
{
    float x = GetInfoScrollerPosition().X;
    float y = GetInfoScrollerPosition().Y - kHudHeight - kHudSmallBuffer;
    return FVector2D(x, y);
}
