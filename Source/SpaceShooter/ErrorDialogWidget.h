// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "ErrorDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UErrorDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;
    void OnClose() override;
	
public:
    UDialogWidget::Type GetType() override;
    
    void SetMessage(const FString& errorMessage);
    
    UPROPERTY(meta = (BindWidget)) class UTextBlock* MessageText;
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
};
