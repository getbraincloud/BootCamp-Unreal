// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "HttpManager.h"
#include "Sockets.h"
#include "Networking.h"
#include "Common/TcpListener.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"


class TwitchHelperCallback
{
public:
    virtual ~TwitchHelperCallback() = default;
    virtual void OnTwitchAuthorizationGranted(const FString& accessToken, const FString& email, const FString& username) {}
    virtual void OnTwitchAuthorizationDenied() {}
    virtual void OnTwitchAuthorizationError(const FString& errorMessage) {}
};


/**
 *
 */
class SPACESHOOTER_API TwitchHelper
{
public:
    TwitchHelper(const FString& redirectURL, const FString& clientId, const FString& clientSecret, TwitchHelperCallback* callback);
    ~TwitchHelper();
    
    void StartLocalCallbackServer(const FString& authState, const FString& localHostAndPort);
    void StopLocalCallbackServer();
    
    FString GetAccessToken();
    FString GetUserEmail();
    FString GetUsername();
    
private:
    void StartHttpListener(const FString& localHostAndPort);
    void StoptHttpListener();
    
    bool ProcessPendingMessage(FSocket* socket, uint32 dataSize, FString& refMessage);
    bool RequestAccessToken(const FString& authCode);
    bool ValidateAccessToken(const FString& accessToken);
    bool RequestUserData();

    FString FormatAuthTokenRequestURL(const FString& authCode);
    
    bool OnAuthCodeListenerCallback(FSocket* socket, const FIPv4Endpoint& endpoint);
    void OnRequestAccessTokenResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void OnValidateAccessTokenResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void OnRequestUserDataResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    FString m_ThreadName;
    FString m_RedirectURL;
    FString m_ClientID;
    FString m_ClientSecret;
    FString m_AuthState;
    FString m_AccessToken;
    FString m_UserEmail;
    FString m_Username;
    TwitchHelperCallback* m_Callback;
    FTcpListener* m_TcpListener;
    FSocket* m_Socket;
};
