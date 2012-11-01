#ifndef __TRANSPORTMANAGER_ITRANSPORTMANAGERLISTENER_HPP__
#define __TRANSPORTMANAGER_ITRANSPORTMANAGERLISTENER_HPP__

#include <vector>

#include "TransportManager/ITransportManager.hpp"
#include "TransportManager/SDeviceInfo.hpp"

namespace NsAppLink
{
    namespace NsTransportManager
    {
        /**
         * @brief Enumeration of send status.
         **/
        enum ESendStatus
        {
            /**
             * @brief Sending frame has succeeded.
             **/
            SendStatusOK,

            /**
             * @brief Timeout occured.
             **/
            SendStatusTimeout,

            /**
             * @brief Sending frame has failed.
             **/
            SendStatusFailed,

            /**
             * @brief Transport manager internal error.
             **/
            SendStatusInternalError,

            /**
             * @brief Unknown error.
             **/
            SendStatusUnknownError
        };

        /**
         * @brief Interface of transport manager listener.
         *
         * Clients of transport manager must inherit this interface
         * in order to register as listener to receive callbacks.
         **/
        class ITransportManagerListener
        {
            /**
             * @brief Destructor.
             **/
            virtual ~ITransportManagerListener(void);

            /**
             * @brief Available devices list updated.
             *
             * @param DeviceList New list of available devices.
             **/
            virtual void onDeviceListUpdated(const tDeviceList & DeviceList);

            /**
             * @brief Device connected.
             *
             * @param ConnectedDevice DeviceInfo of connected device.
             **/
            virtual void onDeviceConnected(const SDeviceInfo & ConnectedDevice);

            /**
             * @brief Device disconnected.
             *
             * @param DisconnectedDevice DeviceInfo of disconnected device.
             **/
            virtual void onDeviceDisconnected(const SDeviceInfo & DisconnectedDevice);

            /**
             * @brief Frame received callbacks.
             *
             * @param ConnectionHandle Connection handle.
             * @param Data Received frame payload data.
             **/
            virtual void onFrameReceived(tConnectionHandle ConnectionHandle, const Blob & Data);

            /**
             * @brief Frame send completed callback.
             *
             * @param ConnectionHandle Connection handle.
             * @param FrameSequenceNumber Sequence numbere of frame that was previously returned by ITransportManager::sendFrame.
             * @param SendStatus Result status.
             **/
            virtual void onFrameSendCompleted(tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus);
        };
    }
}

#endif
