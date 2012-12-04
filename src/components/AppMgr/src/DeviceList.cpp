/**
 * \file DeviceList.cpp
 * \brief Device list
 * \author vsalo
 */

#include "AppMgr/DeviceList.h"

namespace NsAppManager
{
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
     * \brief Copy constructor
     */
    DeviceList::DeviceList(const DeviceList &)
    {
    }
}
