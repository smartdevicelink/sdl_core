#ifndef __TRANSPORTMANAGER_ITRANSPORTMANAGERDATALISTENER_HPP__
#define __TRANSPORTMANAGER_ITRANSPORTMANAGERDATALISTENER_HPP__

#include "TransportManager/ITransportManager.hpp"

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
             * @brief Unknown error.
             **/
            SendStatusUnknownError
        };

        /**
         * @brief Interface of transport manager listener related to data information.
         *
         * Clients of transport manager must inherit this interface
         * in order to register as listener to receive callbacks about data information
         **/
        class ITransportManagerDataListener
        {
            /**
             * @brief Destructor
             *
             **/
            virtual ~ITransportManagerDataListener();

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



#endif // __TRANSPORTMANAGER_ITRANSPORTMANAGERDATALISTENER_HPP__
