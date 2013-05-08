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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_API_VERSION_CONVERTER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_API_VERSION_CONVERTER_H_

#include "utils/macro.h"
#include "SmartObjects/CSmartObject.hpp"

namespace application_manager {
    /**
     * @brief typedef for SharedPtr<CSmartObject> type
     */
    typedef utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::CSmartObject> tSharedPtr;

    /**
     * @brief class APIVersionConverter is intended to provide interfaces
     * for conversation application API from V1, RPC to internal representation which is API V2.
     */
    class APIVersionConverter {
    public:
        /**
         * @brief Class constructor
         */
        APIVersionConverter();

        /**
         * @brief Class destructor
         */
        virtual ~APIVersionConverter();

        /**
         * @brief Converts incoming message from API V1 or RPC to API V2
         * @param smartObj Incoming SmartObject to be converted
         *
         * @return SharedPtr<CSmartObject> for transfered message converted to API V2
         */
        virtual tSharedPtr convertToV2(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& smartObj) = 0;

        /**
         * @brief Converts incoming message from API V2 to API V1 or RPC
         * @param smartObj Incoming SmartObject to be converted
         *
         * @return SharedPtr<CSmartObject> for transfered message converted to API V1 or RPC
         */
        virtual tSharedPtr convertFromV2(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& smartObj) = 0;

        /**
         * @brief Provides information if message is valid after conversion
         *
         * @return TRUE if message is valid, otherwise FALSE.
         */
        virtual bool isMessageValid() const = 0;

    private:
        DISALLOW_COPY_AND_ASSIGN(APIVersionConverter);
    };
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_API_VERSION_CONVERTER_H_
