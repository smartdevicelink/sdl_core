//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef VEHICLEDATAMAPPING_H
#define VEHICLEDATAMAPPING_H

#include <map>
#include <vector>
#include "JSONHandler/SDLRPCObjects/V2/VehicleDataType.h"

namespace log4cplus
{
    class Logger;
}

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
        static log4cplus::Logger mLogger;
    };

}

#endif // VEHICLEDATAMAPPING_H
