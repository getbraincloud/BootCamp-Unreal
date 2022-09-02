// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDInfoScrollerUserWidget.h"
#include "Constants.h"
#include "DialogManager.h"
#include <list>
#include "GameFramework/HUD.h"
#include "SpaceShooterHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API ASpaceShooterHUD : public AHUD, public HUDInfoScrollerCallback
{
	GENERATED_BODY()
    
public:
    ASpaceShooterHUD();
    
protected:
    virtual void BeginPlay() override;
    
public:   
    void SetElapsedTimeHUD(float elapsedTime);
    void UpdateShipDamageHUD(float healthPCT, float shieldPCT);

    void ShowGameOverHUD();
    void HideGameOverHUD();
    
    void ShowGameWonHUD();
    void HideGameWonHUD();

    void ShowLevelHUD(int number, const FString& information = "");
    void HideLevelHUD();
    
    void PushAchievement(class Achievement* achievement);
    void PushLeaderboardEntry(class LeaderboardEntry* leaderboardEntry);
    void PushLevelGoal(const FString& levelGoal);
    void PushPlayerHasAllTimeHighScore();

    void ResetHUD();
        
    UDialogManager* GetDialogManager() const;
    
private:
    void OnHUDInfoScrollerMoveHasCompleted(UHUDInfoScrollerUserWidget* infoScroller) override;
    void OnHUDInfoScrollerHoldCompleted(UHUDInfoScrollerUserWidget* infoScroller) override;

    void PushInfoScrollerData(const HUDInfoScrollerData& infoScrollerData);
    void TryPushingNextInfoScroller();
    void SwapInfoScrollers();
    
    FVector2D GetInfoScrollerPosition();
    FVector2D GetInfoScrollerEnteringPosition();
    FVector2D GetInfoScrollerLeavingPosition();

    template<typename T> T* CreateWidgetAndValidate(const FString& name)
    {
        FString directory = "/Game/Widgets/";
        FString path = directory + name + "." + name + "_C";
        UClass* widgetClass = StaticLoadClass(T::StaticClass(), nullptr, *path);
        check(widgetClass != nullptr);
        
        UUserWidget* userWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
        check(userWidget != nullptr);
        
        userWidget->SetDesiredSizeInViewport(FVector2D(kScreenWidth, kScreenHeight));
        userWidget->SetAnchorsInViewport(FAnchors(0.5f, 0.5f));
        userWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
        userWidget->AddToViewport();
        
        T* castedWidget = Cast<T>(userWidget);
        check(castedWidget != nullptr);

        return castedWidget;
    }
    
    UPROPERTY() UDialogManager* m_DialogManager;
    class UHUDUserWidget* m_HUDWidget;
    UHUDInfoScrollerUserWidget* m_DisplayedInfoScroller;
    UHUDInfoScrollerUserWidget* m_LeavingInfoScroller;
    std::list<HUDInfoScrollerData> m_InfoScrollerData;
    bool m_PlayerHasAllTimeHighScore;
};
