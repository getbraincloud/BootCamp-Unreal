// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Fader.h"
#include "Blueprint/UserWidget.h"
#include "HUDUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UHUDUserWidget : public UUserWidget, public FaderCallback
{
	GENERATED_BODY()
	
protected:
    bool Initialize() override;
    
public:
    void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
    void SetElapsedTime(float elapsedTime);
    void SetHealth(float pct);
    void SetShield(float pct);
    
    void ShowGameOver();
    void HideGameOver();
    
    void ShowGameWon();
    void HideGameWon();

    void ShowLevel(int number, const FString& information);
    void HideLevel();
    
    void SetElapsedTimeColor(const FLinearColor& color);
    
    void Reset();

    class UHUDInfoScrollerUserWidget* GetHUDInfoScrollerWidget0();
    class UHUDInfoScrollerUserWidget* GetHUDInfoScrollerWidget1();

private:
    void ShowInformation(const FString& main, const FString& secondary, bool fadeIn = true);
    void HideInformation(bool fadeOut = true);
    
    void HandleDangerFill(class UProgressBar* fill, class UProgressBar* dangerFill, float pct);
    
    void OnFadeOutHasCompleted(Fader* fader) override;

    UPROPERTY(meta = (BindWidget)) class UTextBlock* ElapsedTime;
    UPROPERTY(meta = (BindWidget)) class UTextBlock* MainInformation;
    UPROPERTY(meta = (BindWidget)) class UTextBlock* SecondaryInformation;
    UPROPERTY(meta = (BindWidget)) class UProgressBar* HealthFill;
    UPROPERTY(meta = (BindWidget)) class UProgressBar* HealthDangerFill;
    UPROPERTY(meta = (BindWidget)) class UProgressBar* ShieldFill;
    UPROPERTY(meta = (BindWidget)) class UProgressBar* ShieldDangerFill;
    UPROPERTY(meta = (BindWidget)) class UHUDInfoScrollerUserWidget* HUDInfoScrollerWidget_0;
    UPROPERTY(meta = (BindWidget)) class UHUDInfoScrollerUserWidget* HUDInfoScrollerWidget_1;

    Fader m_InformationFader;
    float m_DangerAlpha;
    float m_DangerRadians;
    float m_HealthPCT;
    float m_ShieldPCT;
};
