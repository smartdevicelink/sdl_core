#include "AppMgr/ButtonCapabilities.h"
#include "JSONHandler/RPC2Objects/Buttons/GetCapabilitiesResponse.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger ButtonCapabilitiesContainer::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ButtonCapabilities"));

/**
 * \brief Default class constructor
 */
ButtonCapabilitiesContainer::ButtonCapabilitiesContainer()
{
    LOG4CPLUS_INFO_EXT(mLogger, " ButtonCapabilitiesContainer constructed!");
}

/**
 * \brief set button capabilities
 * \param caps button capabilities
 */
void ButtonCapabilitiesContainer::set(const std::vector<AppLinkRPC::ButtonCapabilities>& caps)
{
    if(caps.empty())
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Trying to set empty capabilities set");
        return;
    }

    mButtonCapabilities = caps;
}

/**
 * \brief get button capabilities
 * \return button capabilities
 */
const ButtonCapabilitiesContainer::Capabilities& ButtonCapabilitiesContainer::get() const
{
    return mButtonCapabilities;
}

/**
 * \brief Default class copy constructor
 */
ButtonCapabilitiesContainer::ButtonCapabilitiesContainer(const NsAppManager::ButtonCapabilitiesContainer &)
{
}

}
