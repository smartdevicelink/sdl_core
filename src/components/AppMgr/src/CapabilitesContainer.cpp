/**
 * \file CapabilitiesContainer.cpp
 * \brief Capabilities container
 * \author vsalo
 */

#include "AppMgr/CapabilitesContainer.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

    template<class DeviceCapabilities>
    log4cplus::Logger CapabilitiesContainer<DeviceCapabilities>::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ButtonCapabilities"));

    /**
     * \brief Default class constructor
     */
    template<class DeviceCapabilities>
    CapabilitiesContainer<DeviceCapabilities>::CapabilitiesContainer()
    {
        LOG4CPLUS_INFO_EXT(mLogger, " CapabilitiesContainer constructed!");
    }

    /**
     * \brief Default class destructor
     */
    template<class DeviceCapabilities>
    CapabilitiesContainer<DeviceCapabilities>::~CapabilitiesContainer()
    {
        mCapabilities.clear();
    }

    /**
     * \brief set capabilities
     * \param caps capabilities
     */
    template<class DeviceCapabilities>
    void CapabilitiesContainer<DeviceCapabilities>::set(const CapabilitiesContainer::Capabilities& caps)
    {
        if(caps.empty())
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Trying to set empty capabilities set");
            return;
        }

        mCapabilities = caps;
    }

    /**
     * \brief get capabilities
     * \return capabilities
     */
    template<class DeviceCapabilities>
    const typename CapabilitiesContainer<DeviceCapabilities>::Capabilities& CapabilitiesContainer<DeviceCapabilities>::get() const
    {
        return mCapabilities;
    }

    /**
     * \brief Default class copy constructor
     */
    template<class DeviceCapabilities>
    CapabilitiesContainer<DeviceCapabilities>::CapabilitiesContainer(const CapabilitiesContainer&)
    {
    }

    template class CapabilitiesContainer<NsAppLinkRPC::ButtonCapabilities>;
    template class CapabilitiesContainer<NsAppLinkRPC::HmiZoneCapabilities>;
    template class CapabilitiesContainer<NsAppLinkRPC::VrCapabilities>;
    template class CapabilitiesContainer<NsAppLinkRPC::SpeechCapabilities>;

}
