// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "PlayAgainDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UPlayAgainDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;

public:
    UDialogWidget::Type GetType() override;
    
private:
    UFUNCTION(Category = "CallBack") void OnNewGameButtonClicked();
    UFUNCTION(Category = "CallBack") void OnBrainCloudButtonClicked();
    UFUNCTION(Category = "CallBack") void OnMainMenuButtonClicked();

    UPROPERTY(meta = (BindWidget)) class UButton* NewGameButton;
    UPROPERTY(meta = (BindWidget)) class UButton* BrainCloudButton;
    UPROPERTY(meta = (BindWidget)) class UButton* MainMenuButton;
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
};
