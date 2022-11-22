// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AchievementElementUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UAchievementElementUserWidget : public UUserWidget
{
	GENERATED_BODY()
    
public:
    void Reset();
    
private:    
    UPROPERTY(meta = (BindWidget)) class UTextBlock* DescriptionText;
    UPROPERTY(meta = (BindWidget)) class UTextBlock* StatusText;
};
