// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SPACESHOOTER_API StringUtils
{
public:
    static FString FormatTime(float time);
    
    static FString ParseHttpResponse(const FString& httpResponse, const FString& queryString);
};
