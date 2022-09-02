// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "ChangeUsernameDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UChangeUsernameDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;
    
public:
    UDialogWidget::Type GetType() override;
    
private:
    UFUNCTION(Category = "CallBack") void OnChangeButtonClicked();
    
    UPROPERTY(meta = (BindWidget)) class UEditableTextBox* UsernameInputBox;
    UPROPERTY(meta = (BindWidget)) class UButton* ChangeButton;
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
};
