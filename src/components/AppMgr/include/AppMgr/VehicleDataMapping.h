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

#ifndef VEHICLEDATAMAPPING_H
#define VEHICLEDATAMAPPING_H

#include <map>
#include <vector>
#include "JSONHandler/SDLRPCObjects/V2/VehicleDataType.h"
#include "utils/logger.h"

namespace NsAppManager
{

    class Application;

    /**
     * \brief a VehicleData-registered-app map
     */
    typedef std::multimap<NsSmartDeviceLinkRPCV2::VehicleDataType::VehicleDataTypeInternal, Application*> VehicleDataMap;

    /**
     * \brief a VehicleData-registered-app map iterator
     */
    typedef std::multimap<NsSmartDeviceLinkRPCV2::VehicleDataType::VehicleDataTypeInternal, Application*>::iterator VehicleDataMapIterator;

    /**
     * \brief a VehicleData-registered-app map item
     */
    typedef std::pair<NsSmartDeviceLinkRPCV2::VehicleDataType::VehicleDataTypeInternal, Application*> VehicleDataMapItem;

    /**
     * \brief VehicleDataMapping acts as a mapping of VehicleData to registered application which subscribes to them
     */
    class VehicleDataMapping
    {
    public:

        /**
         * \brief Default class constructor
         */
        VehicleDataMapping();

        /**
         * \brief Default class destructor
         */
        ~VehicleDataMapping();

        /**
         * \brief add a VehicleData to a mapping
         * \param vehicleDataName button name
         * \param app application to map a button to
         * \return false if such subscribe already exists.
         */
        bool addVehicleDataMapping(const NsSmartDeviceLinkRPCV2::VehicleDataType& vehicleDataName, Application* app);

        /**
         * \brief remove a VehicleData from a mapping
         * \param vehicleDataName button name
         * \return false if no such subscribe found.
         */
        bool removeVehicleDataMapping(const NsSmartDeviceLinkRPCV2::VehicleDataType& vehicleDataName, Application* app);

        /**
         * \brief remove an application from a mapping
         * \param app application to remove all associated buttons from mapping
         */
        void removeItem(Application* app);

        /**
         * \brief cleans all the mapping
         */
        void clear();

        /**
         * \brief find a registry item subscribed to VehicleData
         * \param vehicleData VehicleDataTypeInternal value
         * \param result reference to empty vector to store results.
         * \return count of subscribers
         */
        int findRegistryItemsSubscribedToVehicleData(const NsSmartDeviceLinkRPCV2::VehicleDataType& vehicleDataName, std::vector<Application*>& result);

    private:

        /**
         * \brief Copy constructor
         */
        VehicleDataMapping(const VehicleDataMapping&);

        VehicleDataMap    mVehicleDataMapping;
        static log4cxx::LoggerPtr logger_;
    };

}

#endif // VEHICLEDATAMAPPING_H
