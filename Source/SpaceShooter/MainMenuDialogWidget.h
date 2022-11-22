// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "MainMenuDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UMainMenuDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
	
protected:
    bool Initialize() override;

    void OnShow() override;
    
public:    
    UDialogWidget::Type GetType() override;
    
private:
    UFUNCTION(Category = "CallBack") void OnEndlessModeButtonClicked();
    UFUNCTION(Category = "CallBack") void OnHordeModeButtonClicked();
    UFUNCTION(Category = "CallBack") void OnBrainCloudButtonClicked();
    UFUNCTION(Category = "CallBack") void OnExitButtonClicked();

    UPROPERTY(meta = (BindWidget)) class UButton* EndlessModeButton;
    UPROPERTY(meta = (BindWidget)) class UButton* HordeModeButton;
    UPROPERTY(meta = (BindWidget)) class UButton* BrainCloudButton;
    UPROPERTY(meta = (BindWidget)) class UButton* ExitButton;

};
