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

    void LogOut();
    void Reconnect();
    
    void RequestAnonymousAuthentication();
    
private:
    UFUNCTION(Category = "CallBack") void OnAuthenticationCallback(const FString& jsonData);
    UFUNCTION(Category = "CallBack") void OnAuthenticationError(int statusCode, int reasonCode, const FString& jsonError);
    
    UFUNCTION(Category = "CallBack") void OnLogOutCallback(const FString& jsonData);
    UFUNCTION(Category = "CallBack") void OnLogOutError(int statusCode, int reasonCode, const FString& jsonError);
    
    UPROPERTY() UBrainCloudWrapper* m_BrainCloud;
    NetworkCallback* m_Callback;
    NetworkCallbackHandler m_AuthenticationCallback;
    NetworkCallbackHandler m_LogOutCallback;
};
