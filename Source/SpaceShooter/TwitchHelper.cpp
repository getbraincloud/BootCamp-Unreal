// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "TwitchHelper.h"
#include "StringUtils.h"
#include "Common/TcpSocketBuilder.h"
#include "Async/Async.h"
#include "NetworkMessage.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include <string>


TwitchHelper::TwitchHelper(const FString& redirectURL, const FString& clientId, const FString& clientSecret, TwitchHelperCallback* callback) :
    m_RedirectURL(redirectURL),
    m_ClientID(clientId),
    m_ClientSecret(clientSecret),
    m_Callback(callback),
    m_TcpListener(nullptr),
    m_Socket(nullptr)
{
    
}

TwitchHelper::~TwitchHelper()
{
    if (m_TcpListener != nullptr)
    {
        delete m_TcpListener;
        m_TcpListener = nullptr;
    }
}

void TwitchHelper::StartLocalCallbackServer(const FString& authState, const FString& localHostAndPort)
{
    // Initialize the Twitch properties
    m_AccessToken = "";
    m_UserEmail = "";
    m_Username = "";
    m_AuthState = authState;
    
    StartHttpListener(localHostAndPort);
}

void TwitchHelper::StopLocalCallbackServer()
{
    StoptHttpListener();
}

FString TwitchHelper::GetAccessToken()
{
    return m_AccessToken;
}

FString TwitchHelper::GetUserEmail()
{
    return m_UserEmail;
}

FString TwitchHelper::GetUsername()
{
    return m_Username;
}

void TwitchHelper::StartHttpListener(const FString& localHostAndPort)
{
    if (m_TcpListener != nullptr)
        StoptHttpListener();
    
    // Create the endpoint
    FIPv4Endpoint endPoint;
    FIPv4Endpoint::FromHostAndPort(localHostAndPort, endPoint);
    
    // Create the TCP Listener
    m_TcpListener = new FTcpListener(endPoint);
    m_TcpListener->OnConnectionAccepted().BindRaw(this, &TwitchHelper::OnAuthCodeListenerCallback);
}

void TwitchHelper::StoptHttpListener()
{
    UE_LOG(LogTemp, Warning, TEXT("TwitchHelper::StoptHttpListener()"));

    if(m_TcpListener != nullptr)
    {
        m_TcpListener->Stop();
        
        delete m_TcpListener;
        m_TcpListener = nullptr;
    }
}

bool TwitchHelper::ProcessPendingMessage(FSocket* socket, uint32 dataSize, FString& refMessage)
{
    check(socket);
    
    FArrayReaderPtr datagram = MakeShareable(new FArrayReader(true));
    datagram->Init(FMath::Min(dataSize, 65507u), 81920);
    
    int32 bytesRead = 0;

    if (socket->Recv(datagram->GetData(), datagram->Num(), bytesRead))
    {
        char* data = (char*)datagram->GetData();
        data[bytesRead] = '\0';
        refMessage = FString(UTF8_TO_TCHAR(data));
        return true;
    }

    return false;
}

bool TwitchHelper::RequestAccessToken(const FString& authCode)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = FHttpModule::Get().CreateRequest();
    
    FString url = FormatAuthTokenRequestURL(authCode);
    httpRequest->SetURL(url);
    httpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    httpRequest->SetVerb(TEXT("POST"));
    httpRequest->OnProcessRequestComplete().BindRaw(this, &TwitchHelper::OnRequestAccessTokenResponseReceived);
    
    return httpRequest->ProcessRequest();
}

bool TwitchHelper::ValidateAccessToken(const FString& accessToken)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = FHttpModule::Get().CreateRequest();
    
    FString url = "https://id.twitch.tv/oauth2/validate";
    httpRequest->SetURL(url);
    httpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + m_AccessToken);
    httpRequest->SetVerb(TEXT("GET"));
    httpRequest->OnProcessRequestComplete().BindRaw(this, &TwitchHelper::OnValidateAccessTokenResponseReceived);
    
    return httpRequest->ProcessRequest();
}

bool TwitchHelper::RequestUserData()
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httpRequest = FHttpModule::Get().CreateRequest();
    
    FString url = "https://api.twitch.tv/helix/users";
    httpRequest->SetURL(url);
    httpRequest->AppendToHeader(TEXT("Authorization"), TEXT("Bearer ") + m_AccessToken);
    httpRequest->AppendToHeader(TEXT("Client-Id"), m_ClientID);
    httpRequest->SetVerb(TEXT("GET"));
    httpRequest->OnProcessRequestComplete().BindRaw(this, &TwitchHelper::OnRequestUserDataResponseReceived);
    
    return httpRequest->ProcessRequest();
}

FString TwitchHelper::FormatAuthTokenRequestURL(const FString& authCode)
{
    FString url = "https://id.twitch.tv/oauth2/token?client_id=" + m_ClientID
                                             + "&client_secret=" + m_ClientSecret
                                             + "&code=" + authCode
                                             + "&grant_type=authorization_code"
                                             + "&redirect_uri=" + FGenericPlatformHttp::UrlEncode(m_RedirectURL);
    return url;
}

bool TwitchHelper::OnAuthCodeListenerCallback(FSocket* socket, const FIPv4Endpoint& endpoint)
{
    // Sleep this thread to give the socket time to realize that it has pending data
    FPlatformProcess::Sleep(0.01f);

    FString errorMessage;
    bool hasError = false;
    bool hasDenied = false;
    
    // Safety check the socket
    if(socket != nullptr)
    {
        FString messageData;
        FString message;
        uint32 dataSize = 0;
        TArray<uint8> data;
        bool haveMessage = false;
        
        // Does the socket have any pending data?
        while (socket->HasPendingData(dataSize))
        {
            haveMessage = ProcessPendingMessage(socket, dataSize, message);
            if (haveMessage)
            {
                messageData += message;
                message.Empty();
                haveMessage = false;
            }
        }

        // Parse the HTTP response and get the authCode and the state
        FString authCode = StringUtils::ParseHttpResponse(messageData, "code");
        FString authState = StringUtils::ParseHttpResponse(messageData, "state");
        FString error = StringUtils::ParseHttpResponse(messageData, "error");

        // Ensure there isn't an error and that the auth state matches what was sent int
        if(error == "" && authCode.Len() > 0 && authState == m_AuthState)
        {
            // Request an access token, pass in the authCode
            bool success = RequestAccessToken(authCode);
            
            if(!success)
            {
                hasError = true;
                errorMessage = "Error: Access Token request failed.";
            }
        }
        else
        {
            hasError = true;
            
            if(authState != m_AuthState)
                errorMessage = "Error: state value does NOT match authState client initially sent.";
            else if(authCode.Len())
                errorMessage = "Error: auth code is empty.";
            else if(error == "access_denied")
            {
                // The user denied access, not an error
                hasDenied = true;
                hasError = false;
            }
            else if(error != "" && error != "access_denied")
                errorMessage = "Error: " + error;
        }

        // Build a response to send an "ok" back to the browser for the user to see
        FString responseString = "<html><body><b>DONE!</b><br>(You can close this tab/window now)</body></html>";
        FTCHARToUTF8 responseBodyUTF8(*responseString);

        FString responseHeader = FString::Printf(
            TEXT(
                "HTTP/1.1 200 OK\r\n"
                "Connection: close\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n"
                "Content-Length: %d\r\n"
                "\r\n"
            ),
            responseBodyUTF8.Length());

        FString response = responseHeader.Append(responseString);

        FTCHARToUTF8 responseUTF8(*response);
        int32 sentBytes = 0;

        // Send the response and close the socket
        socket->Send((const uint8_t*)responseUTF8.Get(), responseUTF8.Length(), sentBytes);
        socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(socket);
    }
    else
    {
        hasError = true;
        errorMessage = "Error: Socket is NULL.";
    }
    
    // Ensure the callbacks and the StoptHttpListener() method are invoked on the GameThread
    AsyncTask(ENamedThreads::GameThread, [&, hasDenied, hasError, errorMessage]()
    {
        if(hasDenied && m_Callback != nullptr)
            m_Callback->OnTwitchAuthorizationDenied();
        else if(hasError && m_Callback != nullptr) // Is there an error? Invoke the callback
            m_Callback->OnTwitchAuthorizationError(errorMessage);
        
        StoptHttpListener();
    });

    return true;
}

void TwitchHelper::OnRequestAccessTokenResponseReceived(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
    FString errorMessage;
    bool hasError = false;

    if (!response.IsValid() || !bWasSuccessful)
    {
        hasError = true;
        errorMessage = "Error: Failed to get Twitch access token from authorization code.";
    }
    else
    {
        FString responseString = response->GetContentAsString();
        
        // Parse the Json data
        TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(responseString);
        TSharedPtr<FJsonObject> jsonPacket = MakeShareable(new FJsonObject());
        const bool bSuccess = FJsonSerializer::Deserialize(reader, jsonPacket);
        
        if(bSuccess)
        {
            FString accessToken = jsonPacket->GetStringField(TEXT("access_token"));
            FString tokenType = jsonPacket->GetStringField(TEXT("token_type"));
            
            // Verify the access token
            if (accessToken.Len() > 0 && tokenType == "bearer")
            {
                // Set the access token
                m_AccessToken = accessToken;

                // Make an HTTP Request to Validate the access token with Twitch
                bool success = ValidateAccessToken(m_AccessToken);
                
                if(!success)
                {
                    hasError = true;
                    errorMessage = "Error: The HTTP request validate the Twitch access token failed.";
                }
            }
            else
            {
                hasError = true;
                errorMessage = "Error: Failed to get Twitch access token, server response returned an empty access token field.";
            }
        }
    }
    
    if(hasError && m_Callback != nullptr)
    {
        m_Callback->OnTwitchAuthorizationError(errorMessage);
    }
}

void TwitchHelper::OnValidateAccessTokenResponseReceived(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
    FString errorMessage;
    bool hasError = false;

    if (!response.IsValid() || !bWasSuccessful)
    {
        hasError = true;
        errorMessage = "Error: Validating the Twitch access token failed.";
    }
    else
    {
        // If the response code is 200, the access token was validated
        if(response->GetResponseCode() == 200)
        {
            // Make an HTTP Request to user's data from Twitch
            bool success = RequestUserData();
            
            if(!success)
            {
                hasError = true;
                errorMessage = "Error: The HTTP for the Twitch user data failed.";
            }
        }
    }
    
    if(hasError && m_Callback != nullptr)
    {
        m_Callback->OnTwitchAuthorizationError(errorMessage);
    }
}

void TwitchHelper::OnRequestUserDataResponseReceived(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
    FString errorMessage;
    bool hasError = false;
    
    if (!response.IsValid() || !bWasSuccessful)
    {
        hasError = true;
        errorMessage = "Error: Twitch user data request failed.";
    }
    else
    {
        FString responseString = response->GetContentAsString();
        
        // Parse the Json data
        TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(responseString);
        TSharedPtr<FJsonObject> jsonPacket = MakeShareable(new FJsonObject());
        const bool bSuccess = FJsonSerializer::Deserialize(reader, jsonPacket);
        
        if(bSuccess)
        {
            TArray<TSharedPtr<FJsonValue>> data = jsonPacket->GetArrayField(TEXT("data"));
            
            if (data.Num() > 0)
            {
                // Parse the user's email and display name
                m_UserEmail = data[0]->AsObject()->GetStringField(TEXT("email"));
                m_Username = data[0]->AsObject()->GetStringField(TEXT("display_name"));

                // Invoke the callback
                if(m_Callback != nullptr)
                    m_Callback->OnTwitchAuthorizationGranted(m_AccessToken, m_UserEmail, m_Username);
                
                // Return early from this function
                return;
            }
            else
            {
                hasError = true;
                errorMessage = "Error: Failed to extra data json object from Twitch json user data.";
            }
        }
        else
        {
            hasError = true;
            errorMessage = "Error: Failed to parse Twitch json user data.";
        }
    }
    
    if(hasError && m_Callback != nullptr)
    {
        m_Callback->OnTwitchAuthorizationError(errorMessage);
    }
}
