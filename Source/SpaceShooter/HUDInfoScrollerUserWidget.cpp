// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "HUDInfoScrollerUserWidget.h"
#include "LeaderboardEntry.h"
#include "Achievement.h"
#include "Constants.h"
#include "StringUtils.h"
#include "Components/TextBlock.h"


HUDInfoScrollerData::HUDInfoScrollerData() :
    text(""),
    initialPosition(0.0f, 0.0f),
    displayMinimum(0.0f),
    type(Type::Unknown)
{
    
}

HUDInfoScrollerData::HUDInfoScrollerData(const FString& overrideText, FVector2D initialPosition, float displayMinimum) :
    text(overrideText),
    initialPosition(initialPosition),
    displayMinimum(displayMinimum),
    type(Type::Text)
{
    
}

HUDInfoScrollerData::HUDInfoScrollerData(class LeaderboardEntry* leaderboardEntry, FVector2D initialPosition) :
    text("High Score #" + FString::FromInt(leaderboardEntry->GetRank()) + ": " + StringUtils::FormatTime(leaderboardEntry->GetTime())),
    initialPosition(initialPosition),
    displayMinimum(0.0f),
    type(Type::LeaderboardEntry)
{
    
}

HUDInfoScrollerData::HUDInfoScrollerData(class Achievement* achievement, FVector2D initialPosition) :
    text("Achievement: " + achievement->GetTitle()),
    initialPosition(initialPosition),
    displayMinimum(2.5f),
    type(Type::Achievement)
{
    
}

bool UHUDInfoScrollerUserWidget::Initialize()
{    
    bool success = Super::Initialize();
    
    if(!success)
        return false;

    m_Callback = nullptr;
    m_DisplayMinimum = 0.0f;
    
    return true;
}

void UHUDInfoScrollerUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    m_Slider.Tick(InDeltaTime);
    SetRenderTranslation(m_Slider.GetCurrent());


    if (m_DisplayMinimum > 0.0f)
    {
        m_DisplayMinimum -= InDeltaTime;
        if (m_DisplayMinimum < 0.0f)
        {
            m_DisplayMinimum = 0.0f;

            if (m_Callback != nullptr)
                m_Callback->OnHUDInfoScrollerHoldCompleted(this);
        }
    }
}

void UHUDInfoScrollerUserWidget::SetCallback(HUDInfoScrollerCallback* callback)
{
    m_Callback = callback;
}

void UHUDInfoScrollerUserWidget::PushOn(HUDInfoScrollerData& infoScrollerData, const FVector2D& target)
{
    m_DisplayMinimum = infoScrollerData.displayMinimum;;
    OutputText->SetText(FText::FromString(infoScrollerData.text));
    SetRenderTranslation(infoScrollerData.initialPosition);
    SetVisibility(ESlateVisibility::Visible);
    MoveTo(target);
}

void UHUDInfoScrollerUserWidget::PushOff(const FVector2D& target)
{
    MoveTo(target);
}

void UHUDInfoScrollerUserWidget::Clear()
{
    m_DisplayMinimum = 0.0f;
    SetVisibility(ESlateVisibility::Hidden);
}

bool UHUDInfoScrollerUserWidget::IsMoving()
{
    return m_Slider.IsSliding();
}

bool UHUDInfoScrollerUserWidget::CanPush()
{
    return m_DisplayMinimum == 0.0f;
}

void UHUDInfoScrollerUserWidget::OnSlideHasCompleted(class VSlider* slider)
{
    if (m_Callback != nullptr)
        m_Callback->OnHUDInfoScrollerMoveHasCompleted(this);
}
 
void UHUDInfoScrollerUserWidget::MoveTo(const FVector2D& target)
{
    FVector2D start = RenderTransform.Translation;
    float distance = FVector2D::Distance(start, target);
    float time = distance / kHudHighScoreMovementSpeed;
    m_Slider.StartSlide(start, target, time, this);
}
