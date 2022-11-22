// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "LeaderboardsDialogWidget.h"
#include "LeaderboardElementUserWidget.h"
#include "Leaderboard.h"
#include "LeaderboardEntry.h"
#include "SpaceShooterGameModeBase.h"
#include "Components/Button.h"


bool ULeaderboardsDialogWidget::Initialize()
{
    bool success = Super::Initialize();
    
    if(!success)
        return false;
    
    m_LeaderboardEntryElements[0] = LeaderboardElement_0;
    m_LeaderboardEntryElements[1] = LeaderboardElement_1;
    m_LeaderboardEntryElements[2] = LeaderboardElement_2;
    m_LeaderboardEntryElements[3] = LeaderboardElement_3;
    m_LeaderboardEntryElements[4] = LeaderboardElement_4;
    m_LeaderboardEntryElements[5] = LeaderboardElement_5;
    m_LeaderboardEntryElements[6] = LeaderboardElement_6;
    m_LeaderboardEntryElements[7] = LeaderboardElement_7;
    m_LeaderboardEntryElements[8] = LeaderboardElement_8;
    m_LeaderboardEntryElements[9] = LeaderboardElement_9;

    if(SegmentControlLeft)
        SegmentControlLeft->OnClicked.AddUniqueDynamic(this, &ULeaderboardsDialogWidget::OnLeftSegmentButtonClicked);

    if(SegmentControlRight)
        SegmentControlRight->OnClicked.AddUniqueDynamic(this, &ULeaderboardsDialogWidget::OnRightSegmentButtonClicked);
    
    CloseButton->OnClicked.AddUniqueDynamic(this, &UDialogWidget::OnCloseButtonClicked);
    
    return true;
}

void ULeaderboardsDialogWidget::OnShow()
{
    SetSegmentControlState(SegmentControlState::Main);
    
    SetLeaderboardData(kBrainCloudMainLeaderboardID);
}

UDialogWidget::Type ULeaderboardsDialogWidget::GetType()
{
    return UDialogWidget::Type::Leaderboards;
}

void ULeaderboardsDialogWidget::SetSegmentControlState(SegmentControlState segmentControlState)
{
    FString leftTexture = GetTexturePath(SegmentControlState::Main);
    FString rightTexture = GetTexturePath(SegmentControlState::Daily);

    SetButtonTextures(SegmentControlLeft, leftTexture, segmentControlState == SegmentControlState::Main);
    SetButtonTextures(SegmentControlRight, rightTexture, segmentControlState == SegmentControlState::Daily);
}

void ULeaderboardsDialogWidget::SetLeaderboardData(const FString& leaderboardId)
{
    ResetLeaderboardData();
    
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    if(gameMode != nullptr)
    {
        Leaderboard* leaderboard = gameMode->GetLeaderboardsManager()->GetLeaderboardByName(leaderboardId);
        if (leaderboard != nullptr)
        {
            LeaderboardEntry* leaderboardEntry = nullptr;
            for (int i = 0; i < leaderboard->GetCount(); i++)
            {
                leaderboardEntry = leaderboard->GetLeaderboardEntryAtIndex(i);
                if (leaderboardEntry != nullptr && i < kMaxLeaderboardElements)
                    m_LeaderboardEntryElements[i]->SetLeaderboarEntry(leaderboardEntry);
            }
        }
    }
}

void ULeaderboardsDialogWidget::ResetLeaderboardData()
{
    for(int i = 0; i < kMaxLeaderboardElements; i++)
        m_LeaderboardEntryElements[i]->Reset();
}

FString ULeaderboardsDialogWidget::GetTexturePath(SegmentControlState segmentControlState)
{
    FString texture = "/Game/Menu-Sprites/SegmentControl";

    switch (segmentControlState)
    {
        case SegmentControlState::Main:
            texture += "Left-";
            break;

        case SegmentControlState::Daily:
            texture += "Right-";
            break;
    }

    return texture;
}

bool ULeaderboardsDialogWidget::SetButtonTextures(class UButton* button, const FString& path, bool isSelected)
{
    if(button != nullptr)
    {
        FString selectedPath = path + "Selected";
        UObject* selectedTextureObject = LoadObject<UTexture2D>(NULL, *selectedPath, NULL, LOAD_None, NULL);
        if(selectedTextureObject == nullptr)
            return false;
        
        FString unselectedPath = path + "Unselected";
        UObject* unselectedTextureObject = LoadObject<UTexture2D>(NULL, *unselectedPath, NULL, LOAD_None, NULL);
        if(unselectedTextureObject == nullptr)
            return false;

        if(isSelected)
        {
            button->WidgetStyle.Normal.SetResourceObject(selectedTextureObject);
            button->WidgetStyle.Hovered.SetResourceObject(selectedTextureObject);
            button->WidgetStyle.Pressed.SetResourceObject(unselectedTextureObject);
        }
        else
        {
            button->WidgetStyle.Normal.SetResourceObject(unselectedTextureObject);
            button->WidgetStyle.Hovered.SetResourceObject(unselectedTextureObject);
            button->WidgetStyle.Pressed.SetResourceObject(selectedTextureObject);
        }
        
        return true;
    }
    return false;
}

void ULeaderboardsDialogWidget::OnLeftSegmentButtonClicked()
{
    SetSegmentControlState(SegmentControlState::Main);
    
    SetLeaderboardData(kBrainCloudMainLeaderboardID);
}

void ULeaderboardsDialogWidget::OnRightSegmentButtonClicked()
{
    SetSegmentControlState(SegmentControlState::Daily);
    
    SetLeaderboardData(kBrainCloudDailyLeaderboardID);
}
