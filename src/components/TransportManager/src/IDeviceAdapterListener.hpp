#ifndef __TRANSPORTMANAGER_IDEVICEADAPTERLISTENER_HPP__
#define __TRANSPORTMANAGER_IDEVICEADAPTERLISTENER_HPP__

#include <vector>

#include "TransportManager/ITransportManager.hpp"
#include "TransportManager/ITransportManagerListener.hpp"

#include "SInternalDeviceInfo.hpp"

namespace NsAppLink
{
    namespace NsTransportManager
    {
        class IDeviceAdapter;

        /**
         * @brief Interface of device adapter listener.
         *
         * Used to notify transport manager about events in
         * device adapters.
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
             * @param DeviceAdapter Calling device adapter.
             * @param DeviceList New list of available devices.
             **/
            virtual void onDeviceListUpdated(IDeviceAdapter * DeviceAdapter, const tInternalDeviceList & DeviceList);

            /**
             * @brief Device connected callback.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param ConnectedDevice DeviceInfo of connected device.
             **/
            virtual void onDeviceConnected(IDeviceAdapter * DeviceAdapter, const SInternalDeviceInfo & ConnectedDevice);

            /**
             * @brief Device disconnected callback.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param DisconnectedDevice DeviceInfo of disconnected device.
             **/
            virtual void onDeviceDisconnected(IDeviceAdapter * DeviceAdapter, const SInternalDeviceInfo & DisconnectedDevice);

            /**
             * @brief Frame received callback.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param ConnectionHandle Connection handle.
             * @param Data Received frame payload data.
             **/
            virtual void onFrameReceived(IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, const Blob & Data);

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
