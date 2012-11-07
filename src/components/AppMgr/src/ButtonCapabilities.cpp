#include "AppMgr/ButtonCapabilities.h"
#include "JSONHandler/RPC2Objects/Buttons/GetCapabilitiesResponse.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger ButtonCapabilitiesContainer::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ButtonCapabilities"));

ButtonCapabilitiesContainer::ButtonCapabilitiesContainer()
{
    LOG4CPLUS_INFO_EXT(mLogger, " ButtonCapabilitiesContainer constructed!");
}

void ButtonCapabilitiesContainer::set(const std::vector<AppLinkRPC::ButtonCapabilities>& caps)
{
    if(caps.empty())
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Trying to set empty capabilities set");
        return;
    }

    mButtonCapabilities = caps;
}

const ButtonCapabilitiesContainer::Capabilities& ButtonCapabilitiesContainer::get() const
{
    return mButtonCapabilities;
}

ButtonCapabilitiesContainer::ButtonCapabilitiesContainer(const NsAppManager::ButtonCapabilitiesContainer &)
{
}

}
