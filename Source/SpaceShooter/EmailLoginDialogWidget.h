// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "EmailLoginDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UEmailLoginDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;
    void OnClose() override;
    
public:
    UDialogWidget::Type GetType() override;
    
    UFUNCTION(Category = "CallBack") void OnLoginButtonClicked();
    UFUNCTION(Category = "CallBack") void OnAnonymousButtonClicked();
    UFUNCTION(Category = "CallBack") void OnTwitchButtonClicked();

    UPROPERTY(meta = (BindWidget)) class UEditableTextBox* EmailInputBox;
    UPROPERTY(meta = (BindWidget)) class UEditableTextBox* PasswordInputBox;

    UPROPERTY(meta = (BindWidget)) class UButton* LoginButton;
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
    UPROPERTY(meta = (BindWidgetOptional)) class UButton* AnonymousButton;
    UPROPERTY(meta = (BindWidgetOptional)) class UButton* TwitchButton;
};
