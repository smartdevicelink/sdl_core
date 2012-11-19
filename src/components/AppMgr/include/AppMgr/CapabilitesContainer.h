/**
 * \file CapabilitiesContainer.h
 * \brief Capabilities container
 * \author vsalo
 */

#ifndef CAPABILITESCONTAINER_H
#define CAPABILITESCONTAINER_H

#include <vector>

namespace log4cplus
{
    class Logger;
}

#include "JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/GetCapabilitiesResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetCapabilitiesResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/VR/GetCapabilitiesResponse.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/TTS/GetCapabilitiesResponse.h"

namespace NsAppManager
{

    /**
     * \brief CapabilitiesContainer acts as container of button capabilities
     */
    template<class DeviceCapabilities>
    class CapabilitiesContainer
    {
    public:

        /**
         * \brief Capabilities container
         */
        typedef std::vector<DeviceCapabilities> Capabilities;

        /**
         * \brief Default class constructor
         */
        CapabilitiesContainer();

        /**
         * \brief Default class destructor
         */
        ~CapabilitiesContainer();

        /**
         * \brief set capabilities
         * \param caps capabilities
         */
        void set(const Capabilities& caps );

        /**
         * \brief get capabilities
         * \return capabilities
         */
        const Capabilities& get() const;

    private:

        /**
         * \brief Default class copy constructor
         */
        CapabilitiesContainer(const CapabilitiesContainer&);

        Capabilities mCapabilities;
        static log4cplus::Logger mLogger;
    };

}

#endif // CAPABILITESCONTAINER_H
