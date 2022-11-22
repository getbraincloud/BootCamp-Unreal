// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


class SPACESHOOTER_API VSliderCallback
{
public:
    virtual ~VSliderCallback() = default;
    virtual void OnSlideHasCompleted(class VSlider* slider) = 0;
};


/**
 * 
 */
class SPACESHOOTER_API VSlider
{
public:
	VSlider();
    ~VSlider() = default;
    
    void Tick(float DeltaTime);

    void StartSlide(FVector2D start, FVector2D target, float duration, VSliderCallback* callback = nullptr);
    void StartSlide(FVector2D start, FVector2D target, float duration, float delay, VSliderCallback* callback = nullptr);
    
    void Pause();
    void Stop();

    bool IsSliding();
    bool IsDelayed();

    FVector2D GetCurrent();

private:
    VSliderCallback* m_Callback;
    FVector2D m_Current;
    FVector2D m_Start;
    FVector2D m_Target;
    float m_Elapsed;
    float m_Duration;
    float m_Delay;
    bool m_IsSliding;
};
