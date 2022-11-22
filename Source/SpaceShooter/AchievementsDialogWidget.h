// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "Constants.h"
#include "AchievementsDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UAchievementsDialogWidget : public UDialogWidget
{
	GENERATED_BODY()

protected:
    bool Initialize() override;
    void OnShow() override;
    
public:
    UDialogWidget::Type GetType() override;
    
private:
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
    
    UPROPERTY(meta = (BindWidget)) class UAchievementElementUserWidget* AchievementElement_0;
    UPROPERTY(meta = (BindWidget)) class UAchievementElementUserWidget* AchievementElement_1;
    UPROPERTY(meta = (BindWidget)) class UAchievementElementUserWidget* AchievementElement_2;
    UPROPERTY(meta = (BindWidget)) class UAchievementElementUserWidget* AchievementElement_3;
    UPROPERTY(meta = (BindWidget)) class UAchievementElementUserWidget* AchievementElement_4;
    UPROPERTY(meta = (BindWidget)) class UAchievementElementUserWidget* AchievementElement_5;
    UPROPERTY(meta = (BindWidget)) class UAchievementElementUserWidget* AchievementElement_6;
    UPROPERTY(meta = (BindWidget)) class UAchievementElementUserWidget* AchievementElement_7;

    class UAchievementElementUserWidget* m_AchievementElements[kMaxAchievementElements];
};
