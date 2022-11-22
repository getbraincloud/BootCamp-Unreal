// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "LevelSelectDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API ULevelSelectDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
	
protected:
    bool Initialize() override;

public:
    UDialogWidget::Type GetType() override;
    void OnShow() override;
    
private:
    UFUNCTION(Category = "CallBack") void OnLevelOneButtonClicked();
    UFUNCTION(Category = "CallBack") void OnLevelTwoButtonClicked();
    UFUNCTION(Category = "CallBack") void OnLevelThreeButtonClicked();
    UFUNCTION(Category = "CallBack") void OnLevelBossButtonClicked();
    UFUNCTION(Category = "CallBack") void OnMainMenuButtonClicked();
    
    UPROPERTY(meta = (BindWidget)) class UButton* LevelOneButton;
    UPROPERTY(meta = (BindWidget)) class UButton* LevelTwoButton;
    UPROPERTY(meta = (BindWidget)) class UButton* LevelThreeButton;
    UPROPERTY(meta = (BindWidget)) class UButton* LevelBossButton;
    UPROPERTY(meta = (BindWidget)) class UButton* MainMenuButton;
};
