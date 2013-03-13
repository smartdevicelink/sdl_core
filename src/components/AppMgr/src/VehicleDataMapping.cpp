//
// Copyright (c) 2013 Ford Motor Company
//

#include "AppMgr/VehicleDataMapping.h"
#include "AppMgr/RegistryItem.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

    log4cplus::Logger VehicleDataMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("VehicleDataMapping"));

    VehicleDataMapping::VehicleDataMapping()
    {
    }

    VehicleDataMapping::~VehicleDataMapping()
    {
        clear();
    }

    bool VehicleDataMapping::addVehicleDataMapping(const NsAppLinkRPCV2::VehicleDataType& vehicleDataName, 
            Application* app)
    {
        bool result = true;
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Adding a VehicleData to a null registry item");
            return false;
        }
        std::pair<VehicleDataMapIterator, VehicleDataMapIterator> p =
                mVehicleDataMapping.equal_range(vehicleDataName.get());
        if (p.first != p.second)
        {
            for (VehicleDataMapIterator itr = p.first; itr != p.second; itr++)
            {
                if (app->getAppID() == itr->second->getAppID())
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "Already subscribed to VehicleData " 
                            << itr->first << " in app " 
                            << itr->second->getName() );
                    result = false;
                    break;
                }
            }
        }
        if (result)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Subscribed to VehicleData " 
                << vehicleDataName.get() << " in app " 
                << app->getName() );

            mVehicleDataMapping.insert(VehicleDataMapItem(vehicleDataName.get(), app));
        }
        return result;
    }

    bool VehicleDataMapping::removeVehicleDataMapping(const NsAppLinkRPCV2::VehicleDataType& vehicleDataName, Application* app)
    {
        bool result =false;
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Removing VehicleData from a null registry item");
            return result;
        }
        std::pair<VehicleDataMapIterator, VehicleDataMapIterator> p = mVehicleDataMapping.equal_range(vehicleDataName.get());
        if (p.first != p.second)
        {
            for (VehicleDataMapIterator itr = p.first; itr != p.second; itr++)
            {
                if (app->getAppID() == itr->second->getAppID())
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "UnSubscribed from VehicleData "
                            << itr->first << " in app "
                            << itr->second->getName() );

                    mVehicleDataMapping.erase(itr);
                    result = true;
                    break;
                }
            }
        }
        return result;
    }

    void VehicleDataMapping::removeItem(Application *app)
    {
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Trying to remove a null item");
            return;
        }
        for(VehicleDataMap::iterator it = mVehicleDataMapping.begin(); it != mVehicleDataMapping.end(); it++)
        {
            if(it->second->getAppID() == app->getAppID())
            {
                LOG4CPLUS_INFO_EXT(mLogger, "Removed from VehicleData " << it->first << " with app " << it->second->getName() );
                mVehicleDataMapping.erase(it);
            }
        }
    }

    void VehicleDataMapping::clear()
    {
        mVehicleDataMapping.clear();
    }

    int VehicleDataMapping::findRegistryItemsSubscribedToVehicleData(
            const NsAppLinkRPCV2::VehicleDataType& vehicleDataName,
            std::vector<Application*>& result)
    {
        std::pair<VehicleDataMapIterator, VehicleDataMapIterator> p =
                mVehicleDataMapping.equal_range(vehicleDataName.get());
        if (p.first != p.second)
        {
            VehicleDataMapIterator itr;
            for (itr = p.first; itr != p.second; itr++)
            {
                result.push_back(itr->second);
            }
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Found " << result.size() << " subscribers.");
        return result.size();
    }

    VehicleDataMapping::VehicleDataMapping(const VehicleDataMapping &)
    {
    }
}
