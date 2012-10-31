#include "JSONHandler/ActivateApp.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

ActivateApp::ActivateApp()
:RPC2Request( RPC2Marshaller::METHOD_ACTIVATEAPP_REQUEST )
{}
 
ActivateApp::~ActivateApp()
{}

const std::vector<std::string> & ActivateApp::getAppName() const
{
    return mAppNames;
}
    
void ActivateApp::setAppName( const std::vector<std::string> & appNames )
{
    mAppNames = appNames;
}