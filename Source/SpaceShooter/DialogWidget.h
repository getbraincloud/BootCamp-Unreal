// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogWidget.generated.h"

class SPACESHOOTER_API DialogWidgetCallback
{
public:
    virtual ~DialogWidgetCallback() = default;
    virtual void OnDialogShown(class UDialogWidget* dialog) = 0;
    virtual void OnDialogHidden(class UDialogWidget* dialog) = 0;
};

/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UDialogWidget : public UUserWidget
{
	GENERATED_BODY()
    
protected:
    bool Initialize() override;
    
public:
    void Show(bool triggerCallback = true);
    void Hide(bool triggerCallback = true);

    bool IsShowing();
    
    void SetCallback(DialogWidgetCallback* callback);
    
    enum class Type
    {
        Unknown = -1,
        Connecting,
        EmailLogin,
        UniversalLogin,
        MainMenu,
        BrainCloud,
        InGameBrainCloud,
        LevelSelect,
        Pause,
        Achievements,
        Statistics,
        Leaderboards,
        ChangeUsername,
        AttachEmail,
        PlayAgain,
        SubmitScore,
        Error
    };
    
    virtual Type GetType();
    
public:
    UFUNCTION(Category = "CallBack") virtual void OnCloseButtonClicked();
    
protected:
    class ASpaceShooterGameModeBase* GetGameMode() const;
    class UDialogManager* GetDialogManager() const;
    class UNetworkManager* GetNetworkManager() const;
    
    virtual void OnShow();
    virtual void OnHide();
    virtual void OnClose();
    
private:
    DialogWidgetCallback* m_Callback;
    bool m_IsShowing;
};
