// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "PauseDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UPauseDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;

    void OnShow() override;

public:
    UDialogWidget::Type GetType() override;
    
private:
    UFUNCTION(Category = "CallBack") void OnResumeButtonClicked();
    UFUNCTION(Category = "CallBack") void OnBrainCloudButtonClicked();
    UFUNCTION(Category = "CallBack") void OnMainMenuButtonClicked();

    UPROPERTY(meta = (BindWidget)) class UButton* ResumeButton;
    UPROPERTY(meta = (BindWidget)) class UButton* BrainCloudButton;
    UPROPERTY(meta = (BindWidget)) class UButton* MainMenuButton;
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
};
