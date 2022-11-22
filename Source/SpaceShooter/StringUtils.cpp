// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "StringUtils.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include <string>


FString StringUtils::FormatTime(float time)
{
    int minutes = static_cast<int>(time) / 60;
    int seconds = static_cast<int>(time - static_cast<float>(minutes) * 60.0f);
    
    std::string minutesStr = std::to_string(minutes);
    for(size_t i = minutesStr.length(); i < 2; ++i)
        minutesStr.insert(minutesStr.begin(), '0');

    std::string secondsStr = std::to_string(seconds);
    for(size_t i = secondsStr.length(); i < 2; ++i)
        secondsStr.insert(secondsStr.begin(), '0');
    
    std::string returnString(minutesStr + ":" + secondsStr);
    return FString(returnString.c_str());
}

FString StringUtils::ParseHttpResponse(const FString& httpResponse, const FString& queryString)
{
    std::string response = TCHAR_TO_UTF8(*httpResponse);
    std::string query = std::string(TCHAR_TO_UTF8(*queryString)) + "=";
    
    std::size_t start = response.find("?");
    if (start != std::string::npos)
    {
        std::size_t end = response.find(" ", start+1);
        std::size_t queryStart = response.find(query.c_str(), start+1);
        if(queryStart != std::string::npos)
        {
            std::size_t queryEnd = response.find("&", queryStart + query.length());
            if(queryEnd != std::string::npos)
            {
                std::size_t size = queryEnd - (queryStart + query.length());
                std::string queryValue = response.substr(queryStart + query.length(), size);
                
                FString decodedStr = FGenericPlatformHttp::UrlDecode(FString(queryValue.c_str()));
                return decodedStr;
            }
            else
            {
                std::size_t size = end - (queryStart + query.length());
                std::string queryValue = response.substr(queryStart + query.length(), size);
                
                FString decodedStr = FGenericPlatformHttp::UrlDecode(FString(queryValue.c_str()));
                return decodedStr;
            }
        }
    }
    
    return "";
}
