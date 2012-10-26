#include "JSONHandler/OnAppUnregistered.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

OnAppUnregistered::OnAppUnregistered()
:RPC2Notification( RPC2Marshaller::METHOD_ONAPPUNREDISTERED )
{}
 
OnAppUnregistered::~OnAppUnregistered()
{}

void OnAppUnregistered::setAppName(const std::string & appName)
{
    mAppName = appName;
}
        
void OnAppUnregistered::setReason(const AppInterfaceUnregisteredReason& reason)
{
    mReason = reason;
}

std::string OnAppUnregistered::getAppName(void) const
{
    return mAppName;
}
        
AppInterfaceUnregisteredReason OnAppUnregistered::getReason(void) const
{
    return mReason;
}