/**
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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_HMI_MESSAGE_HANDLER_TESTER_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_HMI_MESSAGE_HANDLER_TESTER_H_

#include <list>

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/signals.h"
#include "config_profile/profile.h"

#include "hmi_message_handler/hmi_message_handler.h"
#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "protocol_handler/protocol_handler_impl.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_default.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/mobile_message_handler.h"
#include "application_manager/message_helper.h"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"

namespace am = application_manager;
namespace jsn = NsSmartDeviceLink::NsJSONHandler::strings;
namespace smart = NsSmartDeviceLink::NsSmartObjects;

namespace test {
	/**
	 * @brief Redefinition HMIMessageHandler for test
   */
  class HMIMessageHandlerTester :
    public hmi_message_handler::HMIMessageHandler {
   public:
    static HMIMessageHandlerTester* instance();

    /**
     * @brief Destructor
     */
    ~HMIMessageHandlerTester();

    virtual void SendMessageToHMI(
        hmi_message_handler::MessageSharedPointer message);
    virtual void OnMessageReceived(
        utils::SharedPtr<application_manager::Message> message);
    virtual void OnErrorSending(
        utils::SharedPtr<application_manager::Message> message);
    virtual void AddHMIMessageAdapter(
        hmi_message_handler::HMIMessageAdapter* adapter);
    virtual void RemoveHMIMessageAdapter(
        hmi_message_handler::HMIMessageAdapter* adapter);

    void SignalNewRequestFromMobile();

    bool operator()(
        utils::SharedPtr<smart::SmartObject> in_request);

    bool CheckUntestableRequest();

   private:
		/**
		 * @brief Constructor
		 */
    HMIMessageHandlerTester();

    /**
     * @brief Copy constructor
     *
     * @param first - ref HMIMessageHandlerTester
     *
     */
    HMIMessageHandlerTester(const HMIMessageHandlerTester&);

    /**
     * @brief Operator assignment
     *
     * @param first - ref HMIMessageHandlerTester
     *
     * @return point to HMIMessageHandlerTester
     */
    HMIMessageHandlerTester* operator=(const HMIMessageHandlerTester&);

    utils::SharedPtr<smart::SmartObject> request_;
    utils::SharedPtr<smart::SmartObject> response_;

    hmi_message_handler::MessageSharedPointer* response_from_app_;
    sync_primitives::ConditionalVariable cond_var_;
    sync_primitives::AutoLock aLock_;
    sync_primitives::Lock lock_;

    hmi_message_handler::HMIMessageHandlerImpl* hmi_impl_;

    std::list<hmi_message_handler::MessageSharedPointer> mas_request_to_hmi_;
    std::list<hmi_message_handler::MessageSharedPointer>
        mas_untestable_request_to_hmi;
  };
}  // namespace test

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_HMI_MESSAGE_HANDLER_TESTER_H_
