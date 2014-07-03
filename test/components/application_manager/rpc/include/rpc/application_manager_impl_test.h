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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_APPLICATION_MANAGER_IMPL_TEST_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_APPLICATION_MANAGER_IMPL_TEST_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <stdint.h>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include "application_manager/hmi_command_factory.h"
#include "application_manager/application_manager.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/message.h"
#include "application_manager/request_controller.h"
#include "application_manager/resume_ctrl.h"
#include "protocol_handler/protocol_observer.h"
#include "hmi_message_handler/hmi_message_observer.h"

#include "media_manager/media_manager_impl.h"

#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/device.h"

#include "formatters/CSmartFactory.hpp"

#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"

#include "interfaces/v4_protocol_v1_2_no_extra.h"
#include "interfaces/v4_protocol_v1_2_no_extra_schema.h"

#include "protocol_handler/service_type.h"

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/shared_ptr.h"
#include "utils/message_queue.h"
#include "utils/prioritized_queue.h"
#include "utils/threads/thread.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/lock.h"
#include "utils/singleton.h"

#include "application_manager/application_manager_impl.h"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "application_manager/mobile_message_handler.h"
#include "connection_handler/connection_handler_impl.h"
#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "protocol_handler/protocol_handler_impl.h"

namespace application_manager {
class ApplicationManagerImpl;
}

namespace am = application_manager;
namespace smart = NsSmartDeviceLink::NsSmartObjects;
namespace jsn = NsSmartDeviceLink::NsJSONHandler::strings;

namespace test {

class ApplicationManagerImplTest {
 public:
  ApplicationManagerImplTest();

  ~ApplicationManagerImplTest();

  const std::set<am::ApplicationSharedPtr>& GetApplications();

  const std::list<am::CommandSharedPtr>& GetNotificationList();

  const std::map<const int32_t, const uint32_t>& GetAppIDList();

  bool GetAudioPassThruActive();

  bool GetIsDistractingDriver();

  bool GetIsVrSessionStrated();

  bool GetHMICooperating();

  bool GetIsAllAppsAllowed();

  const media_manager::MediaManager* GetMediaManager();

  const hmi_message_handler::HMIMessageHandler* GetHmiHandler();

  const connection_handler::ConnectionHandler* GetConnectionHandler();

  const protocol_handler::ProtocolHandler* GetProtocolHandler();

  const policy::PolicyManager* GetPolicyManager();

  const am::HMICapabilities& GetHmiCapabilities();

  const hmi_apis::HMI_API* GetHmiSoFactory();

  const mobile_apis::MOBILE_API* GetMobileSoFactory();

  am::mobile_api::AppInterfaceUnregisteredReason::eType
  GetUnregisterReason();

  const am::impl::FromMobileQueue& GetMessagesFromMobile();

  const am::impl::ToMobileQueue& GetMessagesToMobile();

  const am::impl::FromHmiQueue& GetMessagesFromHmi();

  const am::impl::ToHmiQueue& GetMessagesToHmi();

 private:
  am::ApplicationManagerImpl* app_;
};

class HMIMessageHandlerInterceptor :
    public hmi_message_handler::HMIMessageHandler {
 public:
  HMIMessageHandlerInterceptor();

  ~HMIMessageHandlerInterceptor();

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
  std::list<hmi_message_handler::MessageSharedPointer>* GetMasMessage();

 private:
  std::list<hmi_message_handler::MessageSharedPointer> mas_mess;

  DISALLOW_COPY_AND_ASSIGN(HMIMessageHandlerInterceptor);
};

class ProtocolHandlerInterceptor : public protocol_handler::ProtocolHandlerImpl {
 public:
  ProtocolHandlerInterceptor(
      transport_manager::TransportManager* transport_manager_param);

  ~ProtocolHandlerInterceptor();

  void SendMessageToMobileApp(const protocol_handler::RawMessagePtr message,
                              bool final_message) OVERRIDE;

  std::list<protocol_handler::RawMessagePtr>* GetMasRawMessage();

 private:
  ProtocolHandlerInterceptor* operator=(const ProtocolHandlerInterceptor&);
  ProtocolHandlerInterceptor(const ProtocolHandlerInterceptor&);

  std::list<protocol_handler::RawMessagePtr> mas_mess;
};

void RegistrSO(utils::SharedPtr<smart::SmartObject> AppRegRequest);

utils::SharedPtr<protocol_handler::RawMessage>
ConvertSOToRawMess(utils::SharedPtr<smart::SmartObject> so);

void AddDevice(uint32_t);

void RemovedDevice(uint32_t);

}  // namespace test

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_APPLICATION_MANAGER_IMPL_TEST_H_
