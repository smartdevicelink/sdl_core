/**
 * \file VehicleDataMapping.h
 * \brief VehicleData mapping
 * \author akara
 */

#ifndef VEHICLEDATAMAPPING_H
#define VEHICLEDATAMAPPING_H

#include <map>
#include <vector>
#include "JSONHandler/ALRPCObjects/V2/VehicleDataType.h"

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

    class RegistryItem;

    /**
     * \brief a VehicleData-registered-app map
     */
    typedef std::multimap<NsAppLinkRPCV2::VehicleDataType::VehicleDataTypeInternal, RegistryItem*> VehicleDataMap;

    /**
     * \brief a VehicleData-registered-app map iterator
     */
    typedef std::multimap<NsAppLinkRPCV2::VehicleDataType::VehicleDataTypeInternal, RegistryItem*>::iterator VehicleDataMapIterator;

    /**
     * \brief a VehicleData-registered-app map item
     */
    typedef std::pair<NsAppLinkRPCV2::VehicleDataType::VehicleDataTypeInternal, RegistryItem*> VehicleDataMapItem;

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
         */
        void addVehicleDataMapping(const NsAppLinkRPCV2::VehicleDataType& vehicleDataName, RegistryItem* app);

        /**
         * \brief remove a VehicleData from a mapping
         * \param vehicleDataName button name
         */
        void removeVehicleDataMapping(const NsAppLinkRPCV2::VehicleDataType& vehicleDataName, RegistryItem* app);

        /**
         * \brief remove an application from a mapping
         * \param app application to remove all associated buttons from mapping
         */
        void removeItem(RegistryItem* app);

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
        int findRegistryItemsSubscribedToVehicleData(const NsAppLinkRPCV2::VehicleDataType& vehicleDataName, std::vector<RegistryItem*>& result);

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
