/*
 Copyright (c) 2019, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "sdl_rpc_plugin/commands/hmi/on_bc_system_capability_updated_notification.h"

#include "application_manager/app_service_manager.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/rpc_service.h"
#include "interfaces/HMI_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;
namespace commands {

OnBCSystemCapabilityUpdatedNotification::
    OnBCSystemCapabilityUpdatedNotification(
        const application_manager::commands::MessageSharedPtr& message,
        ApplicationManager& application_manager,
        app_mngr::rpc_service::RPCService& rpc_service,
        app_mngr::HMICapabilities& hmi_capabilities,
        policy::PolicyHandlerInterface& policy_handler)
    : NotificationToHMI(message,
                        application_manager,
                        rpc_service,
                        hmi_capabilities,
                        policy_handler) {}

OnBCSystemCapabilityUpdatedNotification::
    ~OnBCSystemCapabilityUpdatedNotification() {}

void OnBCSystemCapabilityUpdatedNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "Sending BasicCommunication.OnSystemCapabilityUpdated "
                "Notification to HMI");

  smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];

  hmi_apis::Common_SystemCapabilityType::eType system_capability_type =
      static_cast<hmi_apis::Common_SystemCapabilityType::eType>(
          msg_params[strings::system_capability]
                    [strings::system_capability_type].asInt());
  switch (system_capability_type) {
    case hmi_apis::Common_SystemCapabilityType::NAVIGATION: {
      smart_objects::SmartObject nav_capability(smart_objects::SmartType_Map);
      bool has_nav_capability = false;
      if (hmi_capabilities_.navigation_capability()) {
        has_nav_capability = true;
        nav_capability = *hmi_capabilities_.navigation_capability();
      }
      has_nav_capability = application_manager_.GetAppServiceManager()
                               .UpdateNavigationCapabilities(nav_capability) ||
                           has_nav_capability;

      if (has_nav_capability) {
        msg_params[strings::system_capability][strings::navigation_capability] =
            nav_capability;
      } else {
        return;
      }
      break;
    }
    case hmi_apis::Common_SystemCapabilityType::PHONE_CALL: {
      if (hmi_capabilities_.phone_capability()) {
        msg_params[strings::system_capability][strings::phone_capability] =
            *hmi_capabilities_.phone_capability();
      } else {
        return;
      }
      break;
    }
    case hmi_apis::Common_SystemCapabilityType::REMOTE_CONTROL: {
      if (!hmi_capabilities_.is_rc_cooperating()) {
        return;
      }
      if (hmi_capabilities_.rc_capability()) {
        msg_params[strings::system_capability][strings::rc_capability] =
            *hmi_capabilities_.rc_capability();
      } else {
        return;
      }
      break;
    }
    case hmi_apis::Common_SystemCapabilityType::VIDEO_STREAMING:
      if (hmi_capabilities_.video_streaming_capability()) {
        msg_params[strings::system_capability]
                  [strings::video_streaming_capability] =
                      *hmi_capabilities_.video_streaming_capability();
      } else {
        return;
      }
      break;
    case hmi_apis::Common_SystemCapabilityType::APP_SERVICES: {
      auto all_services =
          application_manager_.GetAppServiceManager().GetAllServiceRecords();
      auto app_service_caps =
          MessageHelper::CreateAppServiceCapabilities(all_services);

      smart_objects::SmartArray* app_services =
          app_service_caps[strings::app_services].asArray();
      smart_objects::SmartObject& updated_capabilities =
          msg_params[strings::system_capability]
                    [strings::app_services_capabilities][strings::app_services];
      for (size_t i = 0; i < updated_capabilities.length(); i++) {
        std::string service_id =
            updated_capabilities[i][strings::updated_app_service_record]
                                [strings::service_id].asString();
        auto matching_service_predicate = [&service_id](
            const smart_objects::SmartObject& app_service_capability) {
          return service_id ==
                 app_service_capability[strings::updated_app_service_record]
                                       [strings::service_id].asString();
        };

        auto it = std::find_if(app_services->begin(),
                               app_services->end(),
                               matching_service_predicate);
        if (it != app_services->end()) {
          LOG4CXX_DEBUG(logger_,
                        "Replacing updated record with service_id "
                            << service_id);
          app_services->erase(it);
        }
        app_services->push_back(updated_capabilities[i]);
      }
      msg_params[strings::system_capability]
                [strings::app_services_capabilities] = app_service_caps;
      break;
    }
    default:
      return;
  }
  SendNotification();
}

}  // namespace commands
}  // namespace app_service_rpc_plugin
