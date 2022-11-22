// Fill out your copyright notice in the Description page of Project Settings.


#include "Statistic.h"


Statistic::Statistic(const FString& name, const FString& description, int value) :
    m_Name(name),
    m_Description(description),
    m_Value(value),
    m_Increment(0)
{
    
}

void Statistic::ApplyIncrement(int amount)
{
    m_Increment += amount;
    m_Value += amount;
}

FString Statistic::GetName()
{
    return m_Name;
}

FString Statistic::GetDescription()
{
    return m_Description;
}

int Statistic::GetValue()
{
    return m_Value;
}

int Statistic::GetIncrement()
{
    return m_Increment;
}

void Statistic::SetIncrement(int increment)
{
    m_Increment = increment;
}
