// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


class SPACESHOOTER_API FaderCallback
{
public:
    virtual ~FaderCallback() = default;
    virtual void OnFadeInHasCompleted(class Fader* fader) {};
    virtual void OnFadeOutHasCompleted(class Fader* fader) {};
};

/**
 * 
 */
class SPACESHOOTER_API Fader
{
public:
	Fader();
	~Fader() = default;
    
    void Tick(float DeltaTime);

    enum class Type
    {
        None = -1,
        Out,
        In
    };
    
    void StartFade(Type fadeType, float duration, FaderCallback* callback = nullptr);
    void StartFade(Type fadeType, float duration, float delay, FaderCallback* callback = nullptr);
    void Reset(float alpha = 1.0f);
    
    bool IsFading() const;
    bool IsFadingIn() const;
    bool IsFadingOut() const;
    bool IsDelayed() const;
    float GetAlpha() const;

private:
    FaderCallback* m_Callback;
    float m_Alpha;
    float m_Elapsed;
    float m_Duration;
    float m_Delay;
    Type m_Type;
};
