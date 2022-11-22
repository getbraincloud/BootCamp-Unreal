// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "UniversalLoginDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UUniversalLoginDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;
    
public:
    UDialogWidget::Type GetType() override;
    
    UFUNCTION(Category = "CallBack") void OnLoginButtonClicked();

    void OnClose() override;
    
    UPROPERTY(meta = (BindWidget)) class UEditableTextBox* UsernameInputBox;
    UPROPERTY(meta = (BindWidget)) class UEditableTextBox* PasswordInputBox;

    UPROPERTY(meta = (BindWidget)) class UButton* LoginButton;
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
};
