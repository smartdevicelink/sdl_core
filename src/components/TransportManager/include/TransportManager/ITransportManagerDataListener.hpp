/**
 * \file ITransportManagerDataListener.hpp
 * \brief Class ITransportManagerDataListener header.
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __TRANSPORTMANAGER_ITRANSPORTMANAGERDATALISTENER_HPP__
#define __TRANSPORTMANAGER_ITRANSPORTMANAGERDATALISTENER_HPP__

#include <stdint.h>
#include <sys/types.h>

#include "TransportManager/ITransportManager.hpp"

namespace NsSmartDeviceLink
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
             * @brief Invalid incoming parameters.
             **/
            SendStatusInvalidParametersError,

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
         *
         * @interface ITransportManagerDataListener
         *
         * @see @ref components_transportmanager_client_data_transfer
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
             *
             * @see @ref components_transportmanager_client_data_transfer
             **/
            virtual void onFrameReceived(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize);

            /**
             * @brief Frame send completed callback.
             *
             * @param ConnectionHandle Connection handle.
             * @param UserData User data that was previously passed to ITransportManager::sendFrame.
             * @param SendStatus Result status.
             *
             * @see @ref components_transportmanager_client_data_transfer
             **/
            virtual void onFrameSendCompleted(tConnectionHandle ConnectionHandle, int UserData, ESendStatus SendStatus);
        };
    }
}



#endif // __TRANSPORTMANAGER_ITRANSPORTMANAGERDATALISTENER_HPP__
