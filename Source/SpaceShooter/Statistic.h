// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SPACESHOOTER_API Statistic
{
public:
    Statistic(const FString& name, const FString& description, int value);
        
    void ApplyIncrement(int amount = 1);
        
    FString GetName();
    FString GetDescription();
    int GetValue();
    int GetIncrement();
    void SetIncrement(int increment);
        
private:
    FString m_Name;
    FString m_Description;
    int m_Value;
    int m_Increment;
};
