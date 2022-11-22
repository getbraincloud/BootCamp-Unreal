// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "AttachEmailDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UAttachEmailDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;
    
public:
    UDialogWidget::Type GetType() override;
    
private:
    UFUNCTION(Category = "CallBack") void OnAttachButtonClicked();
    
    UPROPERTY(meta = (BindWidget)) class UEditableTextBox* EmailInputBox;
    UPROPERTY(meta = (BindWidget)) class UEditableTextBox* PasswordInputBox;
    
    UPROPERTY(meta = (BindWidget)) class UButton* AttachButton;
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
};
