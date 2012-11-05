#include "AppMgr/ButtonCapabilities.h"
#include "JSONHandler/GetCapabilitiesResponse.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger ButtonCapabilitiesContainer::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ButtonCapabilities"));

ButtonCapabilitiesContainer::ButtonCapabilitiesContainer()
{
    LOG4CPLUS_INFO_EXT(mLogger, " ButtonCapabilitiesContainer constructed!");
}

void ButtonCapabilitiesContainer::set(RPC2Communication::GetCapabilitiesResponse *msg)
{
    std::vector<RPC2Communication::GetCapabilitiesResponse::GetCapabilitiesResponseInternal> result = msg->getCapabilities();
    for( std::vector<RPC2Communication::GetCapabilitiesResponse::GetCapabilitiesResponseInternal>::iterator it = result.begin(); it != result.end(); it++ )
    {
        mButtonCapabilities.push_back(it->capability);
    }
}

const ButtonCapabilitiesContainer::Capabilities& ButtonCapabilitiesContainer::get() const
{
    return mButtonCapabilities;
}

ButtonCapabilitiesContainer::ButtonCapabilitiesContainer(const NsAppManager::ButtonCapabilitiesContainer &)
{
}

}
