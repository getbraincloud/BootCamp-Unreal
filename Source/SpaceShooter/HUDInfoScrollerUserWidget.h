// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VSlider.h"
#include "Blueprint/UserWidget.h"
#include "HUDInfoScrollerUserWidget.generated.h"

class HUDInfoScrollerCallback
{
public:
    virtual ~HUDInfoScrollerCallback() = default;
    virtual void OnHUDInfoScrollerMoveHasCompleted(class UHUDInfoScrollerUserWidget* infoScroller) {}
    virtual void OnHUDInfoScrollerHoldCompleted(class UHUDInfoScrollerUserWidget* infoScroller) {}
};

struct HUDInfoScrollerData
{
    HUDInfoScrollerData();
    HUDInfoScrollerData(const FString& overrideText, FVector2D initialPosition, float displayMinimum = 0.0f);
    HUDInfoScrollerData(class LeaderboardEntry* leaderboardEntry, FVector2D initialPosition);
    
    enum class Type
    {
        Unknown = -1,
        LeaderboardEntry,
        Achievement,
        Text
    };
    
    FString text;
    FVector2D initialPosition;
    float displayMinimum;
    Type type;
};

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UHUDInfoScrollerUserWidget : public UUserWidget, public VSliderCallback
{
	GENERATED_BODY()
	
protected:
    bool Initialize() override;
    
public:
    void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
    void SetCallback(HUDInfoScrollerCallback* callback);
    
    void PushOn(HUDInfoScrollerData& infoScrollerData, const FVector2D& target);
    void PushOff(const FVector2D& target);
        
    void Clear();

    bool IsMoving();
    bool CanPush();
    
protected:
    void OnSlideHasCompleted(class VSlider* slider) override;

private:
    void MoveTo(const FVector2D& target);
    
    UPROPERTY(meta = (BindWidget)) class UTextBlock* OutputText;
    HUDInfoScrollerCallback* m_Callback;
    VSlider m_Slider;
    float m_DisplayMinimum;
};
