/**
 * \file DeviceList.h
 * \brief Device list
 * \author vsalo
 */

#ifndef DEVICELIST_H
#define DEVICELIST_H

#include "ConnectionHandler/CDevice.hpp"

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{
    /**
     * \brief DeviceList acts as a device list
     */
    class DeviceList
    {
    public:

        /**
         * \brief Default class constructor
         */
        DeviceList();

        /**
         * \brief Default class destructor
         */
        ~DeviceList();

        /**
         * \brief set device list
         * \param deviceList device list
         */
        void setDeviceList( const NsConnectionHandler::tDeviceList& deviceList );

        /**
         * \brief get device list
         * \return device list
         */
        const NsConnectionHandler::tDeviceList& getDeviceList() const;

        /**
         * \brief get count of items
         * \return items count
         */
        size_t size() const;

        /**
         * \brief find device associated with the given handle
         * \param handle device handle
         * \return device associated with the given handle
         */
        const NsConnectionHandler::CDevice *findDeviceByHandle(const NsConnectionHandler::tDeviceHandle& handle) const;

        /**
         * \brief find device associated with the given name
         * \param name device name
         * \return device associated with the given name
         */
        const NsConnectionHandler::CDevice *findDeviceByName(const std::string& name) const;

    private:

        /**
         * \brief Copy constructor
         */
        DeviceList(const DeviceList&);

        NsConnectionHandler::tDeviceList mDeviceList;
        static log4cplus::Logger mLogger;
    };
}

#endif // DEVICELIST_H
