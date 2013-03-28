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
#include "JSONHandler/SDLRPCObjects/V2/PresetBankCapabilities.h"
#include "JSONHandler/SDLRPCObjects/V1/SpeechCapabilities.h"
#include "JSONHandler/SDLRPCObjects/V1/VrCapabilities.h"
#include "JSONHandler/SDLRPCObjects/V1/HmiZoneCapabilities.h"
#include "JSONHandler/SDLRPCObjects/V1/ButtonCapabilities.h"
#include "JSONHandler/SDLRPCObjects/V1/DisplayCapabilities.h"

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

        /**
         * \brief clearing underlying container
         */
        void clear();

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
