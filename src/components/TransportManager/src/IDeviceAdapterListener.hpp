/**
 * \file IDeviceAdapterListener.hpp
 * \brief Class IDeviceAdapterListener.
 * Copyright (c) 2013 Ford Motor Company
 */

#ifndef __TRANSPORTMANAGER_IDEVICEADAPTERLISTENER_HPP__
#define __TRANSPORTMANAGER_IDEVICEADAPTERLISTENER_HPP__

#include <stdint.h>
#include <sys/types.h>
#include <vector>

#include "TransportManager/ITransportManager.hpp"
#include "TransportManager/ITransportManagerDataListener.hpp"
#include "TransportManager/ITransportManagerDeviceListener.hpp"

#include "SInternalDeviceInfo.hpp"

namespace NsSmartDeviceLink
{
    namespace NsTransportManager
    {
        class IDeviceAdapter;

        /**
         * @brief Interface of device adapter listener.
         *
         * Used to notify transport manager about events in
         * device adapters.
         * @interface IDeviceAdapterListener
         **/
        class IDeviceAdapterListener
        {
        public:
            /**
             * @brief Destructor.
             **/
            virtual ~IDeviceAdapterListener(void);

            /**
             * @brief Available devices list updated callback.
             *
             * Called when device scanning initiated with scanForNewDevices
             * is completed.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param DeviceList New list of available devices.
             **/
            virtual void onDeviceListUpdated(IDeviceAdapter * DeviceAdapter, const tInternalDeviceList & DeviceList);

            /**
             * @brief Application connected callback.
             *
             * @param DeviceAdapter Device adapter
             * @param ConnectedDevice DeviceInfo of device with connected application.
             * @param Connection Connection handle.
             **/
            virtual void onApplicationConnected(IDeviceAdapter * DeviceAdapter, const SDeviceInfo & ConnectedDevice, const tConnectionHandle ConnectionHandle);

            /**
             * @brief Application disconnected callback.
             *
             * @param DeviceAdapter Device adapter
             * @param ConnectedDevice DeviceInfo of device with connected application.
             * @param Connection Connection handle.
             **/
            virtual void onApplicationDisconnected(IDeviceAdapter * DeviceAdapter, const SDeviceInfo & DisconnectedDevice, const tConnectionHandle ConnectionHandle);

            /**
             * @brief Frame received callback.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param ConnectionHandle Connection handle.
             * @param Data Received frame data.
             * @param DataSize Size of received data in bytes.
             *
             * @note  %Data pointed by parameter Data is valid only until
             *        this callback returns control to the caller. Callback
             *        handler is responsible to copy this data to its own
             *        location if it needs the data after return.
             **/
            virtual void onFrameReceived(IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize);

            /**
             * @brief Frame send completed callback.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param ConnectionHandle Connection handle.
             * @param FrameSequenceNumber Sequence numbere of frame that was previously returned by ITransportManager::sendFrame.
             * @param SendStatus Result status.
             **/
            virtual void onFrameSendCompleted(IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus);
        };
    }
}

#endif
