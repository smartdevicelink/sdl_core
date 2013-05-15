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

namespace NsAppManager
{

    log4cxx::LoggerPtr VehicleDataMapping::logger_ =
        log4cxx::LoggerPtr(log4cxx::Logger::getLogger("VehicleDataMapping"));

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
            LOG4CXX_ERROR_EXT(logger_, " Adding a VehicleData to a null registry item");
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
                    LOG4CXX_INFO_EXT(logger_, "Already subscribed to VehicleData "
                            << itr->first << " in app " 
                            << itr->second->getName() );
                    result = false;
                    break;
                }
            }
        }
        if (result)
        {
            LOG4CXX_INFO_EXT(logger_, "Subscribed to VehicleData "
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
            LOG4CXX_ERROR_EXT(logger_, " Removing VehicleData from a null registry item");
            return result;
        }
        std::pair<VehicleDataMapIterator, VehicleDataMapIterator> p = mVehicleDataMapping.equal_range(vehicleDataName.get());
        if (p.first != p.second)
        {
            for (VehicleDataMapIterator itr = p.first; itr != p.second; itr++)
            {
                if (app->getAppID() == itr->second->getAppID())
                {
                    LOG4CXX_INFO_EXT(logger_, "UnSubscribed from VehicleData "
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
            LOG4CXX_ERROR_EXT(logger_, " Trying to remove a null item");
            return;
        }
        for(VehicleDataMap::iterator it = mVehicleDataMapping.begin(); it != mVehicleDataMapping.end(); it++)
        {
            if(it->second->getAppID() == app->getAppID())
            {
                LOG4CXX_INFO_EXT(logger_, "Removed from VehicleData " << it->first << " with app " << it->second->getName() );
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
        LOG4CXX_INFO_EXT(logger_, "Found " << result.size() << " subscribers.");
        return result.size();
    }

    VehicleDataMapping::VehicleDataMapping(const VehicleDataMapping &)
    {
    }
}
