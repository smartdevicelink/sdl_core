//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

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
        clear();
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
     * \brief clearing underlying container
     */
    template<class DeviceCapabilities>
    void CapabilitiesContainer<DeviceCapabilities>::clear()
    {
        mCapabilities.clear();
    }

    /**
     * \brief Default class copy constructor
     */
    template<class DeviceCapabilities>
    CapabilitiesContainer<DeviceCapabilities>::CapabilitiesContainer(const CapabilitiesContainer&)
    {
    }

    template class CapabilitiesContainer<NsSmartDeviceLinkRPCV2::ButtonCapabilities>;
    template class CapabilitiesContainer<NsSmartDeviceLinkRPCV2::HmiZoneCapabilities>;
    template class CapabilitiesContainer<NsSmartDeviceLinkRPCV2::VrCapabilities>;
    template class CapabilitiesContainer<NsSmartDeviceLinkRPCV2::SpeechCapabilities>;
    template class CapabilitiesContainer<NsSmartDeviceLinkRPCV2::SoftButtonCapabilities>;
}
