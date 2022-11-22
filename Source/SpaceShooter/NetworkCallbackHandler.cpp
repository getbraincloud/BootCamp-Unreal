// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkCallbackHandler.h"
#include "ServiceName.h"
#include "ServiceOperation.h"


NetworkCallbackHandler::NetworkCallbackHandler()
{
}

void NetworkCallbackHandler::serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, const FString& jsonData)
{
    successCallback.ExecuteIfBound(jsonData);
}

void NetworkCallbackHandler::serverError(ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const FString& jsonError)
{
    failureCallback.ExecuteIfBound(statusCode, reasonCode, jsonError);
}
