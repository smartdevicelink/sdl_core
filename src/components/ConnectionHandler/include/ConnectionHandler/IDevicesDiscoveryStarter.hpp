/**
 * \file IDevicesDiscoveryStarter.hpp
 * \brief Starter of devices discovering process.
 * \author AKara
 */

#ifndef IDEVICESDISCOVERYSTARTER_H
#define IDEVICESDISCOVERYSTARTER_H

#include "ConnectionHandler/CDevice.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */
namespace NsConnectionHandler
{
    /**
    * \class IDevicesDiscoveryStarter
    * \brief Starter of devices discovering process class
    */
    class IDevicesDiscoveryStarter
    {
    public:
        /**
         * \brief Method which should start devices discoveryng
         */
        virtual void startDevicesDiscovery()=0;

        /**
         * \brief Connects to all services of device
         * \param deviceHandle Handle of device to connect to
         */
        virtual void connectToDevice( NsConnectionHandler::tDeviceHandle deviceHandle ) = 0;

        virtual void startTransportManager() = 0;

    protected:
        /**
         * \brief Destructor
         */
        virtual ~IDevicesDiscoveryStarter() {};
    };
}/* namespace NsConnectionHandler */

#endif /* IDEVICESDISCOVERYSTARTER_H */
