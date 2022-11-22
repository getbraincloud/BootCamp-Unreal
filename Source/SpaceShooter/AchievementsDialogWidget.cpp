// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "AchievementsDialogWidget.h"
#include "AchievementElementUserWidget.h"
#include "Achievement.h"
#include "AchievementsManager.h"
#include "SpaceShooterGameModeBase.h"
#include "Components/Button.h"


bool UAchievementsDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    m_AchievementElements[0] = AchievementElement_0;
    m_AchievementElements[1] = AchievementElement_1;
    m_AchievementElements[2] = AchievementElement_2;
    m_AchievementElements[3] = AchievementElement_3;
    m_AchievementElements[4] = AchievementElement_4;
    m_AchievementElements[5] = AchievementElement_5;
    m_AchievementElements[6] = AchievementElement_6;
    m_AchievementElements[7] = AchievementElement_7;

    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);
    
    return true;
}

void UAchievementsDialogWidget::OnShow()
{
    for(int i = 0; i < kMaxAchievementElements; i++)
        m_AchievementElements[i]->Reset();
    
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    if(gameMode != nullptr)
    {
        Achievement* achievement = nullptr;
        
        for (int i = 0; i < gameMode->GetAchievementsManager()->GetCount(); i++)
        {
            achievement = gameMode->GetAchievementsManager()->GetAchievementAtIndex(i);
            if (achievement != nullptr && i < kMaxAchievementElements)
                m_AchievementElements[i]->SetAchievement(achievement);
        }
    }
}

UDialogWidget::Type UAchievementsDialogWidget::GetType()
{
    return UDialogWidget::Type::Achievements;
}
