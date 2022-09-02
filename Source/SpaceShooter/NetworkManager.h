// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NetworkCallback.h"
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
    
    bool IsAuthenticated();
    
private:
    NetworkCallback* m_Callback;
};
