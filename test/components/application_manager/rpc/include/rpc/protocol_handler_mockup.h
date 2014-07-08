/**
 * \file protocol_handler.h
 * \brief ProtocolHandlerImpl class header file.
 *
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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_PROTOCOL_HANDLER_MOCKUP_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_PROTOCOL_HANDLER_MOCKUP_H_

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/signals.h"
#include "config_profile/profile.h"
#include "protocol_handler/protocol_handler_impl.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_default.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/mobile_message_handler.h"
#include "application_manager/message_helper.h"
#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "hmi_message_handler/messagebroker_adapter.h"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "protocol/common.h"
#include "json/reader.h"

#include "rpc/init_start_data.h"
#include "rpc/hmi_message_handler_tester.h"

namespace am = application_manager;
namespace jsn = NsSmartDeviceLink::NsJSONHandler::strings;
namespace smart = NsSmartDeviceLink::NsSmartObjects;

namespace test {
  class ProtocolHandlerMockup : public protocol_handler::ProtocolHandlerImpl {
   public:
    static ProtocolHandlerMockup* instance();

    ~ProtocolHandlerMockup();

    bool operator()(
        utils::SharedPtr<smart::SmartObject> in_request,
        utils::SharedPtr<smart::SmartObject> in_responce);

    void SendMessageToMobileApp(const RawMessagePtr message,
                                bool final_message) OVERRIDE;

   private:
    ProtocolHandlerMockup(
        transport_manager::TransportManager* transport_manager_param);

    ProtocolHandlerMockup* operator=(const ProtocolHandlerMockup&);

    ProtocolHandlerMockup(const ProtocolHandlerMockup&);

    utils::SharedPtr<smart::SmartObject> request_;
    utils::SharedPtr<smart::SmartObject> response_;

    utils::SharedPtr<am::Message> response_from_hmi_;
    sync_primitives::ConditionalVariable cond_var_;
    sync_primitives::AutoLock a_lock_;
    sync_primitives::Lock lock_;
  };
}  // namespace test

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_PROTOCOL_HANDLER_MOCKUP_H_
