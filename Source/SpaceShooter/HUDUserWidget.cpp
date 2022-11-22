// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "HUDUserWidget.h"
#include "HUDInfoScrollerUserWidget.h"
#include "Constants.h"
#include "StringUtils.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Color.h"


bool UHUDUserWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    m_DangerAlpha = 0.0f;
    m_DangerRadians = 0.0f;
    m_HealthPCT = 1.0f;
    m_ShieldPCT = 1.0f;
    
    return true;
}

void UHUDUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    m_DangerRadians += kHudDangerFlashIncrement * InDeltaTime;

    if (m_DangerRadians >= PI * 2.0f)
        m_DangerRadians -= PI * 2.0f;
    
    m_DangerAlpha = FMath::Abs(FMath::Sin(m_DangerRadians));

    HandleDangerFill(HealthFill, HealthDangerFill, m_HealthPCT);
    HandleDangerFill(ShieldFill, ShieldDangerFill, m_ShieldPCT);
    
    if (MainInformation->IsVisible() && m_InformationFader.IsFading())
    {
        m_InformationFader.Tick(InDeltaTime);
        MainInformation->SetOpacity(m_InformationFader.GetAlpha());
        SecondaryInformation->SetOpacity(m_InformationFader.GetAlpha());
    }
}

void UHUDUserWidget::SetElapsedTime(float elapsedTime)
{
    FString formattedText = "Time: " + StringUtils::FormatTime(elapsedTime);
    ElapsedTime->SetText(FText::FromString(formattedText));
}

void UHUDUserWidget::SetHealth(float pct)
{
    m_HealthPCT = pct;
    HealthFill->SetPercent(pct);
    HealthDangerFill->SetPercent(pct);
}

void UHUDUserWidget::SetShield(float pct)
{
    m_ShieldPCT = pct;
    ShieldFill->SetPercent(pct);
    ShieldDangerFill->SetPercent(pct);
}

void UHUDUserWidget::ShowGameOver()
{
    UE_LOG(LogTemp, Warning, TEXT("UHeadsUpDisplayWidget::ShowGameOver()"));

    ShowInformation("GAME OVER", "");
}

void UHUDUserWidget::HideGameOver()
{
    UE_LOG(LogTemp, Warning, TEXT("UHeadsUpDisplayWidget::HideGameOver()"));

    HideInformation(false);
}

void UHUDUserWidget::ShowGameWon()
{
    ShowInformation("YOU\'VE SURVIVED...", "JUST BARELY. ENJOY YOUR VICTORY.");
}

void UHUDUserWidget::HideGameWon()
{
    HideInformation(false);
}

void UHUDUserWidget::ShowLevel(int number, const FString& information)
{
    if(number == -1)
        ShowInformation("SURVIVE", "As long as possible");
    else
        ShowInformation("LEVEL " + FString::FromInt(number), information);
}

void UHUDUserWidget::HideLevel()
{
    HideInformation();
}

void UHUDUserWidget::SetElapsedTimeColor(const FLinearColor& color)
{
    ElapsedTime->SetColorAndOpacity(FSlateColor(color));
}

void UHUDUserWidget::Reset()
{
    FString text = "Time: 00:00";
    ElapsedTime->SetText(FText::FromString(text));
    
    SetElapsedTimeColor(FColor(255, 255, 255, 255));
    
    SetHealth(1.0f);
    SetShield(1.0f);
    
    HideGameOver();
}

UHUDInfoScrollerUserWidget* UHUDUserWidget::GetHUDInfoScrollerWidget0()
{
    return HUDInfoScrollerWidget_0;
}

UHUDInfoScrollerUserWidget* UHUDUserWidget::GetHUDInfoScrollerWidget1()
{
    return HUDInfoScrollerWidget_1;
}

void UHUDUserWidget::ShowInformation(const FString& main, const FString& secondary, bool fadeIn)
{
    if (!MainInformation->IsVisible())
    {
        MainInformation->SetVisibility(ESlateVisibility::Visible);
        MainInformation->SetText(FText::FromString(main));

        SecondaryInformation->SetVisibility(ESlateVisibility::Visible);
        SecondaryInformation->SetText(FText::FromString(secondary));

        if (fadeIn)
        {
            MainInformation->SetOpacity(0.0f);
            SecondaryInformation->SetOpacity(0.0f);

            m_InformationFader.StartFade(Fader::Type::In, 0.5f);
        }
        else
        {
            MainInformation->SetOpacity(1.0f);
            SecondaryInformation->SetOpacity(1.0f);
        }
    }
}

void UHUDUserWidget::HideInformation(bool fadeOut)
{
    if (fadeOut)
        m_InformationFader.StartFade(Fader::Type::Out, 0.5f, this);
    else
        OnFadeOutHasCompleted(&m_InformationFader);
}

void UHUDUserWidget::HandleDangerFill(class UProgressBar* fill, class UProgressBar* dangerFill, float pct)
{
    if (pct <= 0.25f)
    {
        dangerFill->SetFillColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, m_DangerAlpha));
        fill->SetFillColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
    }
    else
    {
        dangerFill->SetFillColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
        fill->SetFillColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }
}

void UHUDUserWidget::OnFadeOutHasCompleted(Fader* fader)
{   
    if (fader == &m_InformationFader)
    {
        MainInformation->SetVisibility(ESlateVisibility::Hidden);
        SecondaryInformation->SetVisibility(ESlateVisibility::Hidden);
    }
}
