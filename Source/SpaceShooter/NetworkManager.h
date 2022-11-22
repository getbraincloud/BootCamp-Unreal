// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NetworkCallback.h"
#include "NetworkCallbackHandler.h"
#include "BrainCloudWrapper.h"
#include "UObject/NoExportTypes.h"
#include "NetworkManager.generated.h"


/**
 * 
 */
UCLASS()
class SPACESHOOTER_API UNetworkManager : public UObject
{
	GENERATED_BODY()
    
public:
    UNetworkManager();
    ~UNetworkManager() = default;
    
    void Tick(float DeltaTime);
    
    void SetCallback(NetworkCallback* callback);
    
    bool HasAuthenticatedPreviously();
    bool IsAuthenticated();
    bool IsUsernameSaved();

    void LogOut();
    void Reconnect();
    
    void RequestAnonymousAuthentication();
    void RequestUniversalAuthentication(const FString& userID, const FString& password);
    void RequestEmailAuthentication(const FString& email, const FString& password);
    
    void UpdateUsername(const FString& username);
    FString GetUsername();
    
    void RequestLeaderboard(const FString& leaderboardId);
    void RequestLeaderboard(const FString& leaderboardId, int startIndex, int endIndex);
    
    void PostScoreToLeaderboard(const FString& leaderboardId, float time);
    void PostScoreToLeaderboard(const FString& leaderboardId, float time, const FString& nickname);
    
private:
    UFUNCTION(Category = "CallBack") void OnAuthenticationCallback(const FString& jsonData);
    UFUNCTION(Category = "CallBack") void OnAuthenticationError(int statusCode, int reasonCode, const FString& jsonError);
    
    UFUNCTION(Category = "CallBack") void OnLogOutCallback(const FString& jsonData);
    UFUNCTION(Category = "CallBack") void OnLogOutError(int statusCode, int reasonCode, const FString& jsonError);
    
    UFUNCTION(Category = "CallBack") void OnUpdateUsernameCallback(const FString& jsonData);
    UFUNCTION(Category = "CallBack") void OnUpdateUsernameError(int statusCode, int reasonCode, const FString& jsonError);
    
    UFUNCTION(Category = "CallBack") void OnGetGlobalLeaderboardCallback(const FString& jsonData);
    UFUNCTION(Category = "CallBack") void OnGetGlobalLeaderboardError(int statusCode, int reasonCode, const FString& jsonError);
    
    UFUNCTION(Category = "CallBack") void OnPostScoreToLeaderboardCallback(const FString& jsonData);
    UFUNCTION(Category = "CallBack") void OnPostScoreToLeaderboardError(int statusCode, int reasonCode, const FString& jsonError);
    
    UPROPERTY() UBrainCloudWrapper* m_BrainCloud;
    NetworkCallback* m_Callback;
    NetworkCallbackHandler m_AuthenticationCallback;
    NetworkCallbackHandler m_LogOutCallback;
    NetworkCallbackHandler m_UpdateUsernameCallback;
    NetworkCallbackHandler m_GetGlobalLeaderboardCallback;
    NetworkCallbackHandler m_PostScoreToLeaderboardCallback;
};
