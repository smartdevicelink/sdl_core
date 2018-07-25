/*
 Copyright (c) 2014, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

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

#include "application_manager/commands/command_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/application_manager.h"

namespace application_manager {

namespace {
struct AppExtensionPredicate {
  AppExtensionUID uid;
  bool operator()(const ApplicationSharedPtr app) {
    return app ? (app->QueryInterface(uid).use_count() != 0) : false;
  }
};
}

namespace commands {

CREATE_LOGGERPTR_LOCAL(CommandImpl::logger_, "Commands")

const int32_t CommandImpl::hmi_protocol_type_ = 1;
const int32_t CommandImpl::mobile_protocol_type_ = 0;
const int32_t CommandImpl::protocol_version_ = 3;

CommandImpl::CommandImpl(const MessageSharedPtr& message,
                         ApplicationManager& application_manager,
                         rpc_service::RPCService& rpc_service,
                         HMICapabilities& hmi_capabilities,
                         policy::PolicyHandlerInterface& policy_handler)
    : message_(message)
    , default_timeout_(application_manager.get_settings().default_timeout())
    , allowed_to_terminate_(true)
    , application_manager_(application_manager)
    , rpc_service_(rpc_service)
    , hmi_capabilities_(hmi_capabilities)
    , policy_handler_(policy_handler) {}

CommandImpl::~CommandImpl() {
  CleanUp();
}

bool CommandImpl::CheckPermissions() {
  return true;
}

bool CommandImpl::Init() {
  return true;
}

bool CommandImpl::CleanUp() {
  return true;
}

void CommandImpl::Run() {}

uint32_t CommandImpl::default_timeout() const {
  return default_timeout_;
}

uint32_t CommandImpl::correlation_id() const {
  return (*message_)[strings::params][strings::correlation_id].asUInt();
}

int32_t CommandImpl::function_id() const {
  return (*message_)[strings::params][strings::function_id].asInt();
}

uint32_t CommandImpl::connection_key() const {
  return (*message_)[strings::params][strings::connection_key].asUInt();
}

void CommandImpl::onTimeOut() {}

bool CommandImpl::AllowedToTerminate() {
  return allowed_to_terminate_;
}

void CommandImpl::SetAllowedToTerminate(const bool allowed) {
  allowed_to_terminate_ = allowed;
}

bool CommandImpl::ReplaceMobileWithHMIAppId(
    NsSmartDeviceLink::NsSmartObjects::SmartObject& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (message.keyExists(strings::app_id)) {
    ApplicationSharedPtr application =
        application_manager_.application(message[strings::app_id].asUInt());
    if (!application) {
      LOG4CXX_ERROR(logger_, "Substitution mobile --> HMI id is failed.");
      return false;
    }
    LOG4CXX_DEBUG(logger_,
                  "ReplaceMobileWithHMIAppId from "
                      << message[strings::app_id].asInt() << " to "
                      << application->hmi_app_id());
    message[strings::app_id] = application->hmi_app_id();
  } else {
    switch (message.getType()) {
      case smart_objects::SmartType::SmartType_Array: {
        smart_objects::SmartArray* message_array = message.asArray();
        smart_objects::SmartArray::iterator it = message_array->begin();
        for (; it != message_array->end(); ++it) {
          if (!ReplaceMobileWithHMIAppId(*it)) {
            break;
          }
        }
        break;
      }
      case smart_objects::SmartType::SmartType_Map: {
        std::set<std::string> keys = message.enumerate();
        std::set<std::string>::const_iterator key = keys.begin();
        for (; key != keys.end(); ++key) {
          std::string k = *key;
          if (!ReplaceMobileWithHMIAppId(message[*key])) {
            break;
          }
        }
        break;
      }
      default: { break; }
    }
  }

  return true;
}

DEPRECATED void CommandImpl::ReplaceMobileByHMIAppId(
    NsSmartDeviceLink::NsSmartObjects::SmartObject& message) {
  if (!ReplaceMobileWithHMIAppId(message)) {
    LOG4CXX_ERROR(logger_, "Substitution mobile --> HMI id is failed.");
  }
}

bool CommandImpl::ReplaceHMIWithMobileAppId(
    NsSmartDeviceLink::NsSmartObjects::SmartObject& message) {
  if (message.keyExists(strings::app_id)) {
    ApplicationSharedPtr application =
        application_manager_.application_by_hmi_app(
            message[strings::app_id].asUInt());

    if (!application) {
      LOG4CXX_ERROR(logger_, "Substitution HMI --> mobile id is failed.");
      return false;
    }
    LOG4CXX_DEBUG(logger_,
                  "ReplaceHMIWithMobileAppId from "
                      << message[strings::app_id].asInt() << " to "
                      << application->app_id());
    message[strings::app_id] = application->app_id();
  } else {
    switch (message.getType()) {
      case smart_objects::SmartType::SmartType_Array: {
        smart_objects::SmartArray* message_array = message.asArray();
        smart_objects::SmartArray::iterator it = message_array->begin();
        for (; it != message_array->end(); ++it) {
          if (!ReplaceHMIWithMobileAppId(*it)) {
            break;
          }
        }
        break;
      }
      case smart_objects::SmartType::SmartType_Map: {
        std::set<std::string> keys = message.enumerate();
        std::set<std::string>::const_iterator key = keys.begin();
        for (; key != keys.end(); ++key) {
          if (!ReplaceHMIWithMobileAppId(message[*key])) {
            break;
          }
        }
        break;
      }
      default: { break; }
    }
  }

  return true;
}

DEPRECATED void CommandImpl::ReplaceHMIByMobileAppId(
    NsSmartDeviceLink::NsSmartObjects::SmartObject& message) {
  if (!ReplaceHMIWithMobileAppId(message)) {
    LOG4CXX_ERROR(logger_, "Substitution HMI --> mobile id is failed.");
  }
}

}  // namespace commands
}  // namespace application_manager
