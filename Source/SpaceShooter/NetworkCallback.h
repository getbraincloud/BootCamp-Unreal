// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <vector>

/**
 * 
 */
class SPACESHOOTER_API NetworkCallback
{
public:
    NetworkCallback() = default;
    virtual ~NetworkCallback() = default;
    
    virtual void OnAuthenticationRequestCompleted() {}
    virtual void OnAuthenticationRequestFailed(const FString& errorMessage) {}
    
    virtual void OnLogOutRequestCompleted() {}
    virtual void OnLogOutRequestFailed() {}
    
    //TODO: Add Network callback methods here
};
