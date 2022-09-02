// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "ConnectingDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UConnectingDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
	
protected:
    bool Initialize() override;
    
public:
    void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
    UDialogWidget::Type GetType() override;
    
private:
    UPROPERTY(meta = (BindWidget)) class UTextBlock* Dots;
    
    int m_DotsCount;
    float m_DotsTimer;
};
