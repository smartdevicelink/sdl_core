#include "JSONHandler/ResetGlobalProperties.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

ResetGlobalProperties::ResetGlobalProperties()
:RPC2Request( RPC2Marshaller::METHOD_RESET_GLOBAL_PROPERTIES_REQUEST )
{}
 
ResetGlobalProperties::~ResetGlobalProperties()
{}

void ResetGlobalProperties::setProperty(const std::vector<GlobalProperty>& property)
{
    mProperty = property;
}
        
const std::vector<GlobalProperty>& ResetGlobalProperties::getProperty() const
{
    return mProperty;
}