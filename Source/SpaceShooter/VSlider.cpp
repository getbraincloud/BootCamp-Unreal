// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "VSlider.h"

VSlider::VSlider() :
    m_Callback(nullptr),
    m_Current(FVector2D::ZeroVector),
    m_Start(FVector2D::ZeroVector),
    m_Target(FVector2D::ZeroVector),
    m_Elapsed(0.0f),
    m_Duration(0.0f),
    m_Delay(0.0f),
    m_IsSliding(false)
{
}

void VSlider::Tick(float DeltaTime)
{
    if (m_IsSliding)
    {
        if (m_Delay > 0.0f)
        {
            m_Delay -= DeltaTime;
            if (m_Delay <= 0.0f)
                m_Delay = 0.0f;
            else
                return;
        }

        if (m_Duration > 0.0f)
        {
            m_Elapsed += DeltaTime;
            if (m_Elapsed >= m_Duration)
            {
                m_Elapsed = m_Duration;
                m_IsSliding = false;
            }

            m_Current = m_Start + (m_Target - m_Start) * (m_Elapsed / m_Duration);
            
            if (m_Elapsed == m_Duration && m_Callback != nullptr)
                m_Callback->OnSlideHasCompleted(this);
        }
    }
}

void VSlider::StartSlide(FVector2D start, FVector2D target, float duration, VSliderCallback* callback)
{
    StartSlide(start, target, duration, 0.0f, callback);
}

void VSlider::StartSlide(FVector2D start, FVector2D target, float duration, float delay, VSliderCallback* callback)
{
    m_Callback = callback;
    m_Start = start;
    m_Current = m_Start;
    m_Target = target;
    m_Duration = duration;
    m_Elapsed = 0.0f;
    m_Delay = delay;
    m_IsSliding = true;
}

void VSlider::Pause()
{
    m_IsSliding = false;
}

void VSlider::Stop()
{
    m_Start = FVector2D::ZeroVector;
    m_Current = FVector2D::ZeroVector;
    m_Target = FVector2D::ZeroVector;
    m_Duration = 0.0f;
    m_Elapsed = 0.0f;
    m_Delay = 0.0f;
    m_IsSliding = false;
}

bool VSlider::IsSliding()
{
    return m_IsSliding;
}

bool VSlider::IsDelayed()
{
    return m_Delay > 0.0f && m_IsSliding;
}

FVector2D VSlider::GetCurrent()
{
    return m_Current;
}
