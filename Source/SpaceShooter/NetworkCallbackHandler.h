// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IServerCallback.h"

/**
 * 
 */
class SPACESHOOTER_API NetworkCallbackHandler final : public IServerCallback
{
public:
	NetworkCallbackHandler();

    DECLARE_DELEGATE_OneParam(SuccessCallback, const FString&)
    SuccessCallback successCallback;
    
    DECLARE_DELEGATE_ThreeParams(FailureCallback, int, int, const FString&)
    FailureCallback failureCallback;
    
private:
    void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, const FString& jsonData) override;
    void serverError(ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const FString& jsonError) override;
};
