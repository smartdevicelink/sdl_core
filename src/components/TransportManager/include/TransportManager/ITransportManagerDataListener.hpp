#ifndef __TRANSPORTMANAGER_ITRANSPORTMANAGERDATALISTENER_HPP__
#define __TRANSPORTMANAGER_ITRANSPORTMANAGERDATALISTENER_HPP__

#include <stdint.h>
#include <sys/types.h>

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
         * @brief Interface of transport manager listener related to data information.
         *
         * Clients of transport manager must inherit this interface
         * in order to register as listener to receive callbacks about data information
         **/
        class ITransportManagerDataListener
        {
        public:
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
             * @param DataSize Size of data in bytes.
             **/
            virtual void onFrameReceived(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize);

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
