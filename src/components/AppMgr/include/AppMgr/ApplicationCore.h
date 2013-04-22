//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef SRC_COMPONENTS_APPMGR_INCLUDE_APPMGR_APPLICATIONCORE_H_
#define SRC_COMPONENTS_APPMGR_INCLUDE_APPMGR_APPLICATIONCORE_H_

#include "AppMgrCore.h"
#include "SmartObjects/CSmartObject.hpp"

namespace NsAppManager {
    /**
     * @brief typedef for TSharedPtr<CSmartObject> type
     */
    typedef NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::CSmartObject> tSharedPtr;

    /**
     * @brief class ApplicationCore is intended to provide interfaces for serializing/deserializing between
     * AppMngJSON data to CSmartObject and vice versa.
     */
    class ApplicationCore {
    public:
        /**
         * @brief Class constructor
         */
        ApplicationCore();

        /**
         * @brief Default class destructor
         */
        virtual ~ApplicationCore();

      /**
       * @brief Converts incoming message from NsAppManager::Message to the TSharedPtr<CSmartObject>
       * @param message Incoming NsAppManager::Message message to be converted
       *
       * @return TSharedPtr<CSmartObject> for transfered message
       */
      virtual tSharedPtr processInMessage(const Message& message) = 0;

      /**
       * @brief Converts incoming message from TSharedPtr<CSmartObject> type to the NsAppManager::Message type
       * @param message Incoming TSharedPtr<CSmartObject>
       *
       * @return AppMngrCore::Message for transfered message
       */
      virtual Message processOutMessage(const tSharedPtr message) = 0;

    private:
        /**
         * @brief Copy constructor is private to avoid misusing
         */
        ApplicationCore(const ApplicationCore& appCore);
    };
}  // namespace NsAppManager

#endif  // SRC_COMPONENTS_APPMGR_INCLUDE_APPMGR_APPLICATIONCORE_H_
