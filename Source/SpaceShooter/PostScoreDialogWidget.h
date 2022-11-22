// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "PostScoreDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UPostScoreDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;
    
public:
    UDialogWidget::Type GetType() override;
	
    void Set(float time);
    
private:
    UFUNCTION(Category = "CallBack") void OnSubmitButtonClicked();
    
    UPROPERTY(meta = (BindWidget)) class UEditableTextBox* NameInputBox;
    
    UPROPERTY(meta = (BindWidget)) class UButton* SubmitButton;
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
    
    float m_Time;
};
