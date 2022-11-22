// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "NetworkManager.h"
#include "Constants.h"


UNetworkManager::UNetworkManager() :
    m_BrainCloud(nullptr),
    m_Callback(nullptr)
{
    m_BrainCloud = CreateDefaultSubobject<UBrainCloudWrapper>("BrainCloudWrapper");
    m_BrainCloud->initialize(kBrainCloudServer, kBrainCloudAppSecret, kBrainCloudAppID, kBrainCloudAppVersion);
    
    FString version = m_BrainCloud->getClient()->getBrainCloudClientVersion();
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::UNetworkManager() BrainCloud client version %s"), *version);
    
    // Setup the callback functions
    m_AuthenticationCallback.successCallback.BindUObject(this, &UNetworkManager::OnAuthenticationCallback);
    m_AuthenticationCallback.failureCallback.BindUObject(this, &UNetworkManager::OnAuthenticationError);
    
    m_LogOutCallback.successCallback.BindUObject(this, &UNetworkManager::OnLogOutCallback);
    m_LogOutCallback.failureCallback.BindUObject(this, &UNetworkManager::OnLogOutError);
    
    m_UpdateUsernameCallback.successCallback.BindUObject(this, &UNetworkManager::OnUpdateUsernameCallback);
    m_UpdateUsernameCallback.failureCallback.BindUObject(this, &UNetworkManager::OnUpdateUsernameError);
    
    m_GetGlobalLeaderboardCallback.successCallback.BindUObject(this, &UNetworkManager::OnGetGlobalLeaderboardCallback);
    m_GetGlobalLeaderboardCallback.failureCallback.BindUObject(this, &UNetworkManager::OnGetGlobalLeaderboardError);
    
    m_PostScoreToLeaderboardCallback.successCallback.BindUObject(this, &UNetworkManager::OnPostScoreToLeaderboardCallback);
    m_PostScoreToLeaderboardCallback.failureCallback.BindUObject(this, &UNetworkManager::OnPostScoreToLeaderboardError);
    
    m_GetGlobalEntityLevelDataCallback.successCallback.BindUObject(this, &UNetworkManager::OnGlobalEntityLevelDataCallback);
    m_GetGlobalEntityLevelDataCallback.failureCallback.BindUObject(this, &UNetworkManager::OnGlobalEntityLevelDataError);
    
    m_ReadUserStatisticsCallback.successCallback.BindUObject(this, &UNetworkManager::OnReadUserStatisticsCallback);
    m_ReadUserStatisticsCallback.failureCallback.BindUObject(this, &UNetworkManager::OnReadUserStatisticsError);
    
    m_IncrementUserStatsCallback.successCallback.BindUObject(this, &UNetworkManager::OnIncrementUserStatsCallback);
    m_IncrementUserStatsCallback.failureCallback.BindUObject(this, &UNetworkManager::OnIncrementUserStatsError);
    
    m_ReadAchievementsCallback.successCallback.BindUObject(this, &UNetworkManager::OnReadAchievementsCallback);
    m_ReadAchievementsCallback.failureCallback.BindUObject(this, &UNetworkManager::OnReadAchievementsError);
}

void UNetworkManager::Tick(float DeltaTime)
{
    m_BrainCloud->runCallbacks();
}

void UNetworkManager::SetCallback(NetworkCallback* callback)
{
    m_Callback = callback;
}

bool UNetworkManager::HasAuthenticatedPreviously()
{
    return m_BrainCloud->getStoredProfileId() != "" && m_BrainCloud->getStoredAnonymousId() != "";
}

bool UNetworkManager::IsAuthenticated()
{
    return m_BrainCloud->getClient()->isAuthenticated();
}

bool UNetworkManager::IsUsernameSaved()
{
    return GetUsername() != "";
}

void UNetworkManager::LogOut()
{
    if (IsAuthenticated())
    {
        m_BrainCloud->getPlayerStateService()->logout(&m_LogOutCallback);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::RequestLogOut() Failed: user is not authenticated."));
        
        if (m_Callback != nullptr)
            m_Callback->OnLogOutRequestFailed();
    }
}

void UNetworkManager::Reconnect()
{
    m_BrainCloud->reconnect(&m_AuthenticationCallback);
}

void UNetworkManager::RequestAnonymousAuthentication()
{
    m_BrainCloud->authenticateAnonymous(&m_AuthenticationCallback);
}

void UNetworkManager::RequestUniversalAuthentication(const FString& userID, const FString& password)
{
    m_BrainCloud->authenticateUniversal(userID, password, true, &m_AuthenticationCallback);
}

void UNetworkManager::RequestEmailAuthentication(const FString& email, const FString& password)
{
    m_BrainCloud->authenticateEmailPassword(email, password, true, &m_AuthenticationCallback);
}

void UNetworkManager::UpdateUsername(const FString& username)
{
    if (IsAuthenticated())
    {
        m_BrainCloud->getPlayerStateService()->updateName(username, &m_UpdateUsernameCallback);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::UpdateUsername() Failed: user is not authenticated."));
        
        if (m_Callback != nullptr)
            m_Callback->OnUpdateUsernameRequestFailed();
    }
}

FString UNetworkManager::GetUsername()
{
    return m_BrainCloud->getPlayerStateService()->getUserName();
}

void UNetworkManager::RequestLeaderboard(const FString& leaderboardId)
{
    RequestLeaderboard(leaderboardId, kBrainCloudDefaultMinHighScoreIndex, kBrainCloudDefaultMaxHighScoreIndex);
}

void UNetworkManager::RequestLeaderboard(const FString& leaderboardId, int startIndex, int endIndex)
{
    if (IsAuthenticated())
    {
        m_BrainCloud->getLeaderboardService()->getGlobalLeaderboardPage(leaderboardId, ESortOrder::HIGH_TO_LOW, startIndex, endIndex, &m_GetGlobalLeaderboardCallback);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::RequestLeaderboard() Failed: user is not authenticated."));
        
        if (m_Callback != nullptr)
            m_Callback->OnLeaderboardRequestFailed();
    }
}

void UNetworkManager::PostScoreToLeaderboard(const FString& leaderboardId, float time)
{
    PostScoreToLeaderboard(leaderboardId, time, GetUsername());
}

void UNetworkManager::PostScoreToLeaderboard(const FString& leaderboardId, float time, const FString& nickname)
{
    if (IsAuthenticated())
    {
        // Convert the time from seconds to milleseconds
        long score = static_cast<long>(time * 1000.0f);
        FString jsonOtherData = "{\"nickname\":\"" + nickname + "\"}";
        
        m_BrainCloud->getLeaderboardService()->postScore(leaderboardId, score, jsonOtherData, &m_PostScoreToLeaderboardCallback);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::PostScoreToLeaderboard() Failed: user is not authenticated."));
        
        if (m_Callback != nullptr)
            m_Callback->OnPostScoreToLeaderboardRequestFailed();
    }
}

void UNetworkManager::RequestGlobalEntityLevelData()
{
    if (IsAuthenticated())
    {
        m_BrainCloud->getGlobalEntityService()->getListByIndexedId(kBrainCloudLevelDataIndexedID, 5, &m_GetGlobalEntityLevelDataCallback);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::RequestGlobalEntityLevelData() Failed: user is not authenticated."));
        
        if (m_Callback != nullptr)
            m_Callback->OnGlobalEntityLevelDataRequestFailed();
    }
}

void UNetworkManager::RequestUserStatistics()
{
    if (IsAuthenticated())
    {
        m_BrainCloud->getPlayerStatisticsService()->readAllUserStats(&m_ReadUserStatisticsCallback);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::RequestUserStatistics() Failed: user is not authenticated."));
        
        if (m_Callback != nullptr)
            m_Callback->OnUserStatisticsRequestFailed();
    }
}

void UNetworkManager::IncrementUserStatistics(const FString& jsonDataToSync)
{
    if (IsAuthenticated())
    {
        m_BrainCloud->getPlayerStatisticsService()->incrementUserStats(jsonDataToSync, &m_IncrementUserStatsCallback);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::IncrementUserStatistics() Failed: user is not authenticated."));
        
        if (m_Callback != nullptr)
            m_Callback->OnIncrementUserStatisticsFailed();
    }
}

void UNetworkManager::RequestAchievements()
{
    if (IsAuthenticated())
    {
        m_BrainCloud->getGamificationService()->readAchievements(&m_ReadAchievementsCallback, true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::RequestAchievements() Failed: user is not authenticated."));
        
        if (m_Callback != nullptr)
            m_Callback->OnReadAchievementsRequestFailed();
    }
}

void UNetworkManager::OnAuthenticationCallback(const FString& jsonData)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnAuthenticationCallback(%s)"), *jsonData);
    
    if(m_Callback != nullptr)
        m_Callback->OnAuthenticationRequestCompleted();
}

void UNetworkManager::OnAuthenticationError(int statusCode, int reasonCode, const FString& jsonError)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnAuthenticationError(%s)"), *jsonError);
    
    FString errorMessage = "Authentication failed. ";
    
    if(m_Callback != nullptr)
        m_Callback->OnAuthenticationRequestFailed(errorMessage);
}

void UNetworkManager::OnLogOutCallback(const FString& jsonData)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnLogOutCallback(%s)"), *jsonData);
        
    // The user logged out, clear the persisted data related to their account
    m_BrainCloud->resetStoredAnonymousId();
    m_BrainCloud->resetStoredProfileId();
    
    if (m_Callback != nullptr)
        m_Callback->OnLogOutRequestCompleted();
}

void UNetworkManager::OnLogOutError(int statusCode, int reasonCode, const FString& jsonError)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnLogOutError(%s)"), *jsonError);
    
    if (m_Callback != nullptr)
        m_Callback->OnLogOutRequestFailed();
}

void UNetworkManager::OnUpdateUsernameCallback(const FString& jsonData)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnUpdateUsernameCallback(%s)"), *jsonData);
    
    if (m_Callback != nullptr)
        m_Callback->OnUpdateUsernameRequestCompleted();
}

void UNetworkManager::OnUpdateUsernameError(int statusCode, int reasonCode, const FString& jsonError)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnUpdateUsernameError(%s)"), *jsonError);
    
    if (m_Callback != nullptr)
        m_Callback->OnUpdateUsernameRequestFailed();
}

void UNetworkManager::OnGetGlobalLeaderboardCallback(const FString& jsonData)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnGetGlobalLeaderboardCallback(%s)"), *jsonData);
    
    std::vector<LeaderboardEntry> leaderboardEntries;
    FString leaderboardID;
    
    // Parse the response jsonData
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(jsonData);
    TSharedPtr<FJsonObject> jsonPacket = MakeShareable(new FJsonObject());
    const bool bSuccess = FJsonSerializer::Deserialize(reader, jsonPacket);
    
    if(bSuccess)
    {
        TSharedPtr<FJsonObject> data = jsonPacket->GetObjectField(TEXT("data"));
        TArray<TSharedPtr<FJsonValue>> leaderboard = data->GetArrayField(TEXT("leaderboard"));
        
        int rank = 0;
        FString nickname;
        int ms = 0;
        float time = 0.0f;
        
        // Loop through and get all the leaderboard entries in the leaderboard object
        for (int i = 0; i < leaderboard.Num(); i++)
        {
            TSharedPtr<FJsonObject> extraData = leaderboard[i]->AsObject()->GetObjectField(TEXT("data"));
            nickname = extraData->GetStringField(TEXT("nickname"));
            
            rank = leaderboard[i]->AsObject()->GetIntegerField(TEXT("rank"));
            ms = leaderboard[i]->AsObject()->GetIntegerField(TEXT("score"));
            time = (float)ms / 1000.0f;
            
            leaderboardEntries.emplace_back(nickname, time, rank);
        }
        
        // Get the leaderboardID
        leaderboardID = data->GetStringField(TEXT("leaderboardId"));
    }
    
    Leaderboard leaderboard(leaderboardID, leaderboardEntries);
    
    if (m_Callback != nullptr)
        m_Callback->OnLeaderboardRequestCompleted(leaderboard);
}

void UNetworkManager::OnGetGlobalLeaderboardError(int statusCode, int reasonCode, const FString& jsonError)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnGetGlobalLeaderboardError(%s)"), *jsonError);
    
    if (m_Callback != nullptr)
        m_Callback->OnLeaderboardRequestFailed();
}

void UNetworkManager::OnPostScoreToLeaderboardCallback(const FString& jsonData)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnPostScoreToLeaderboardCallback(%s)"), *jsonData);
    
    if (m_Callback != nullptr)
        m_Callback->OnPostScoreToLeaderboardRequestCompleted();
}

void UNetworkManager::OnPostScoreToLeaderboardError(int statusCode, int reasonCode, const FString& jsonError)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnPostScoreToLeaderboardError(%s)"), *jsonError);
    
    if (m_Callback != nullptr)
        m_Callback->OnPostScoreToLeaderboardRequestFailed();
}

void UNetworkManager::OnGlobalEntityLevelDataCallback(const FString& jsonData)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnGlobalEntityLevelDataCallback(%s)"), *jsonData);
    
    std::vector<LevelData> levelData;
    
    // Parse the response jsonData
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(jsonData);
    TSharedPtr<FJsonObject> jsonPacket = MakeShareable(new FJsonObject());
    const bool bSuccess = FJsonSerializer::Deserialize(reader, jsonPacket);
    
    if(bSuccess)
    {
        FString entityType;
        FString entityID;
        
        TSharedPtr<FJsonObject> data = jsonPacket->GetObjectField(TEXT("data"));
        TArray<TSharedPtr<FJsonValue>> entityList = data->GetArrayField(TEXT("entityList"));
        
        for (int i = 0; i < entityList.Num(); i++)
        {
            entityType = entityList[i]->AsObject()->GetStringField(TEXT("entityType"));
            entityID = entityList[i]->AsObject()->GetStringField(TEXT("entityId"));
            
            TSharedPtr<FJsonObject> entityData = entityList[i]->AsObject()->GetObjectField(TEXT("data"));
            TSharedPtr<FJsonObject> levelJson = entityData->GetObjectField(TEXT("level"));
            
            levelData.emplace_back(entityType, entityID, levelJson);
        }
    }
    
    if (m_Callback != nullptr)
        m_Callback->OnGlobalEntityLevelDataRequestCompleted(levelData);
}

void UNetworkManager::OnGlobalEntityLevelDataError(int statusCode, int reasonCode, const FString& jsonError)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnGlobalEntityLevelDataError(%s)"), *jsonError);
    
    if (m_Callback != nullptr)
        m_Callback->OnGlobalEntityLevelDataRequestFailed();
}

void UNetworkManager::OnReadUserStatisticsCallback(const FString& jsonData)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnReadUserStatsCallback(%s)"), *jsonData);
        
    std::vector<Statistic> statisticsList;
    
    // Parse the response jsonData
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(jsonData);
    TSharedPtr<FJsonObject> jsonPacket = MakeShareable(new FJsonObject());
    const bool bSuccess = FJsonSerializer::Deserialize(reader, jsonPacket);
        
    if(bSuccess)
    {
        TSharedPtr<FJsonObject> data = jsonPacket->GetObjectField(TEXT("data"));
        TSharedPtr<FJsonObject> statistics = data->GetObjectField(TEXT("statistics"));
                
        int value = 0;
                
        for (auto currJsonValue = statistics->Values.CreateConstIterator(); currJsonValue; ++currJsonValue)
        {
            FString statisticName = (*currJsonValue).Key;
            FString statisticDescription = kBrainCloudStatisticDescriptions.at(statisticName);
            value = static_cast<int>(((*currJsonValue).Value->AsNumber()));
            statisticsList.emplace_back(statisticName, statisticDescription, value);
        }
    }
    
    if (m_Callback != nullptr)
        m_Callback->OnUserStatisticsRequestCompleted(statisticsList);
}

void UNetworkManager::OnReadUserStatisticsError(int statusCode, int reasonCode, const FString& jsonError)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnReadUserStatsError(%s)"), *jsonError);
    
    if (m_Callback != nullptr)
        m_Callback->OnUserStatisticsRequestFailed();
}

void UNetworkManager::OnIncrementUserStatsCallback(const FString& jsonData)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnIncrementUserStatsCallback(%s)"), *jsonData);
        
    std::vector<Statistic> statisticsList;

     // Parse the response jsonData
     TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(jsonData);
     TSharedPtr<FJsonObject> jsonPacket = MakeShareable(new FJsonObject());
     const bool bSuccess = FJsonSerializer::Deserialize(reader, jsonPacket);

     if(bSuccess)
     {
         TSharedPtr<FJsonObject> data = jsonPacket->GetObjectField(TEXT("data"));
         TSharedPtr<FJsonObject> statistics = data->GetObjectField(TEXT("statistics"));

         int value = 0;

         for (auto currJsonValue = statistics->Values.CreateConstIterator(); currJsonValue; ++currJsonValue)
         {
             FString statisticName = (*currJsonValue).Key;
             FString statisticDescription = kBrainCloudStatisticDescriptions.at(statisticName);
             value = static_cast<int>(((*currJsonValue).Value->AsNumber()));
             statisticsList.emplace_back(statisticName, statisticDescription, value);
         }
     }
    
    if (m_Callback != nullptr)
        m_Callback->OnIncrementUserStatisticsCompleted(statisticsList);
}

void UNetworkManager::OnIncrementUserStatsError(int statusCode, int reasonCode, const FString& jsonError)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnIncrementUserStatsError(%s)"), *jsonError);
    
    if (m_Callback != nullptr)
        m_Callback->OnIncrementUserStatisticsFailed();
}

void UNetworkManager::OnReadAchievementsCallback(const FString& jsonData)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnReadAchievementsCallback(%s)"), *jsonData);
    
    std::vector<Achievement> achievementsList;
    
    // Parse the response jsonData
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(jsonData);
    TSharedPtr<FJsonObject> jsonPacket = MakeShareable(new FJsonObject());
    const bool bSuccess = FJsonSerializer::Deserialize(reader, jsonPacket);
        
    if(bSuccess)
    {
        TSharedPtr<FJsonObject> data = jsonPacket->GetObjectField(TEXT("data"));
        TArray<TSharedPtr<FJsonValue>> achievements = data->GetArrayField(TEXT("achievements"));
        
        FString achievementID;
        FString title;
        FString description;
        FString status;
        int32 sortOrder = -1;
        
        for (int i = 0; i < achievements.Num(); i++)
        {
            achievementID = achievements[i]->AsObject()->GetStringField(TEXT("id"));
            title = achievements[i]->AsObject()->GetStringField(TEXT("title"));
            description = achievements[i]->AsObject()->GetStringField(TEXT("description"));
            status = achievements[i]->AsObject()->GetStringField(TEXT("status"));
            
            TSharedPtr<FJsonObject> extraData = achievements[i]->AsObject()->GetObjectField(TEXT("extraData"));
            sortOrder = extraData->GetIntegerField(TEXT("sort"));
            
            achievementsList.emplace_back(achievementID, title, description, status, sortOrder);
        }
        
        if (achievementsList.size() > 0)
        {
            std::sort(achievementsList.begin(), achievementsList.end(), [ ](const Achievement& lhs, const Achievement& rhs)
                      {
                return lhs.GetSortOrder() < rhs.GetSortOrder();
            });
        }
    }
        
    if (m_Callback != nullptr)
        m_Callback->OnReadAchievementsRequestCompleted(achievementsList);
}

void UNetworkManager::OnReadAchievementsError(int statusCode, int reasonCode, const FString& jsonError)
{
    UE_LOG(LogTemp, Warning, TEXT("UNetworkManager::OnReadAchievementsError(%s)"), *jsonError);
    
    if (m_Callback != nullptr)
        m_Callback->OnReadAchievementsRequestFailed();
}






