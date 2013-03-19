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

    bool VehicleDataMapping::addVehicleDataMapping(const NsSmartDeviceLinkRPCV2::VehicleDataType& vehicleDataName, 
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

    bool VehicleDataMapping::removeVehicleDataMapping(const NsSmartDeviceLinkRPCV2::VehicleDataType& vehicleDataName, Application* app)
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
            const NsSmartDeviceLinkRPCV2::VehicleDataType& vehicleDataName,
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
