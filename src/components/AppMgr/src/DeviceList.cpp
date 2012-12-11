/**
 * \file DeviceList.cpp
 * \brief Device list
 * \author vsalo
 */

#include "AppMgr/DeviceList.h"

namespace NsAppManager
{
    log4cplus::Logger DeviceList::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("DeviceList"));

    /**
     * \brief Default class constructor
     */
    DeviceList::DeviceList()
    {
    }

    /**
     * \brief Default class destructor
     */
    DeviceList::~DeviceList()
    {
    }

    /**
     * \brief set device list
     * \param deviceList device list
     */
    void DeviceList::setDeviceList(const NsConnectionHandler::tDeviceList &deviceList)
    {
        mDeviceList = deviceList;
    }

    /**
     * \brief get device list
     * \return device list
     */
    const NsConnectionHandler::tDeviceList &DeviceList::getDeviceList() const
    {
        return mDeviceList;
    }

    /**
     * \brief get count of items
     * \return items count
     */
    size_t DeviceList::size() const
    {
        return mDeviceList.size();
    }

    /**
     * \brief find device associated with the given handle
     * \param handle device handle
     * \return device associated with the given handle
     */
    const NsConnectionHandler::CDevice* DeviceList::findDeviceByHandle(const NsConnectionHandler::tDeviceHandle &handle) const
    {
        NsConnectionHandler::tDeviceList::const_iterator it = mDeviceList.find(handle);
        if(it != mDeviceList.end())
        {
            return &it->second;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Device " << handle << " not found in subscribed." );
        return 0;
    }

    /**
     * \brief find device associated with the given name
     * \param name device name
     * \return device associated with the given name
     */
    const NsConnectionHandler::CDevice *DeviceList::findDeviceByName(const std::string &name) const
    {
        for(NsConnectionHandler::tDeviceList::const_iterator it = mDeviceList.begin(); it != mDeviceList.end(); it++)
        {
            const NsConnectionHandler::CDevice& device = it->second;
            if(device.getUserFriendlyName() == name)
            {
                return &device;
            }
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Device " << name << " not found in subscribed." );
        return 0;
    }

    /**
     * \brief Copy constructor
     */
    DeviceList::DeviceList(const DeviceList &)
    {
    }
}
