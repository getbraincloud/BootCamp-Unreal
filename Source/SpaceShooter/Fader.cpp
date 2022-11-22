// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "Fader.h"

Fader::Fader() :
    m_Callback(nullptr),
    m_Alpha(0.0f),
    m_Elapsed(0.0f),
    m_Duration(0.0f),
    m_Delay(0.0f),
    m_Type(Type::None)
{
}

void Fader::Tick(float DeltaTime)
{
    if (m_Type != Type::None)
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
                m_Elapsed = m_Duration;

            m_Alpha = (m_Type == Type::Out) ? (1.0f - (m_Elapsed / m_Duration)) : (m_Elapsed / m_Duration);

            if (m_Elapsed == m_Duration)
            {
                if(m_Callback != nullptr)
                {
                    if(m_Type == Type::Out)
                        m_Callback->OnFadeOutHasCompleted(this);
                    else if(m_Type == Type::In)
                        m_Callback->OnFadeInHasCompleted(this);
                }
                
                m_Type = Type::None;
            }
        }
    }
}

void Fader::StartFade(Type fadeType, float duration, FaderCallback* callback)
{
    StartFade(fadeType, duration, 0.0f, callback);
}

void Fader::StartFade(Type fadeType, float duration, float delay, FaderCallback* callback)
{
    m_Callback = callback;
    m_Type = fadeType;
    m_Alpha = m_Type == Type::Out ? 1.0f : 0.0f;
    m_Duration = duration;
    m_Elapsed = 0.0f;
    m_Delay = delay;
}

void Fader::Reset(float alpha)
{
    m_Type = Type::None;
    m_Alpha = alpha;
    m_Duration = 0.0f;
    m_Elapsed = 0.0f;
    m_Delay = 0.0f;
}

bool Fader::IsFading() const
{
    return m_Duration > 0.0f && m_Type != Type::None;
}

bool Fader::IsFadingIn() const
{
    return IsFading() && m_Type == Type::In;
}

bool Fader::IsFadingOut() const
{
    return IsFading() && m_Type == Type::Out;
}

bool Fader::IsDelayed() const
{
    return m_Delay > 0.0f && IsFading();
}

float Fader::GetAlpha() const
{
    return m_Alpha;
}
