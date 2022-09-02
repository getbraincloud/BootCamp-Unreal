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
}

void UNetworkManager::Tick(float DeltaTime)
{
    m_BrainCloud->runCallbacks();
}

void UNetworkManager::SetCallback(NetworkCallback* callback)
{
    m_Callback = callback;
}

bool UNetworkManager::IsAuthenticated()
{
    return false;
}
