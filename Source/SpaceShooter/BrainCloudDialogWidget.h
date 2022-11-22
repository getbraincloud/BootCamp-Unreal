// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "BrainCloudDialogWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UBrainCloudDialogWidget : public UDialogWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;
    
    void OnShow() override;

public:
    UDialogWidget::Type GetType() override;
    
    void Refresh();

private:
    UFUNCTION(Category = "CallBack") void OnAttachEmailButtonClicked();
    UFUNCTION(Category = "CallBack") void OnChangeUsernameButtonClicked();
    UFUNCTION(Category = "CallBack") void OnLeaderboardsButtonClicked();
    UFUNCTION(Category = "CallBack") void OnStatisticsButtonClicked();
    UFUNCTION(Category = "CallBack") void OnAchievementsButtonClicked();
    UFUNCTION(Category = "CallBack") void OnLogOutButtonClicked();
    UFUNCTION(Category = "CallBack") void OnLogInButtonClicked();
    
    
    UPROPERTY(meta = (BindWidget)) class UButton* AttachEmailButton;
    UPROPERTY(meta = (BindWidget)) class UButton* ChangeUsernameButton;
    UPROPERTY(meta = (BindWidget)) class UButton* LeaderboardsButton;
    UPROPERTY(meta = (BindWidget)) class UButton* StatisticsButton;
    UPROPERTY(meta = (BindWidget)) class UButton* AchievementsButton;
    UPROPERTY(meta = (BindWidget)) class UButton* LogOutButton;
    UPROPERTY(meta = (BindWidget)) class UButton* LogInButton;
    UPROPERTY(meta = (BindWidget)) class UButton* CloseButton;
    UPROPERTY(meta = (BindWidget)) class UVerticalBox* LoggedInBox;
    UPROPERTY(meta = (BindWidget)) class UVerticalBox* LoggedOutBox;
};
