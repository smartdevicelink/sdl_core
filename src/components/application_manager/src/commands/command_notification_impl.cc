/*
 Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/command_notification_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"

#include "utils/shared_ptr.h"
#include "utils/make_shared.h"

namespace application_manager {

namespace commands {

namespace {

struct ParamsDeleter {
  ParamsDeleter(smart_objects::SmartObject& params,
                const std::string& log_message)
      : params_(params), log_message_(log_message) {}

  void operator()(const RPCParams::value_type& value) {
    if (params_.keyExists(value)) {
      params_.erase(value);
#ifdef ENABLE_LOG
      CREATE_LOGGERPTR_LOCAL(logger, "Commands");
      LOG4CXX_DEBUG(logger, log_message_ << value);
#endif  // ENABLE_LOG
    }
  }

 private:
  smart_objects::SmartObject& params_;
  std::string log_message_;
};

}  // namespace

CommandNotificationImpl::CommandNotificationImpl(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandImpl(message, application_manager) {}

CommandNotificationImpl::~CommandNotificationImpl() {}

bool CommandNotificationImpl::Init() {
  return true;
}

bool CommandNotificationImpl::CleanUp() {
  return true;
}

bool CommandNotificationImpl::CheckAllowedParameters(
    const MessageSharedPtr message) const {
  LOG4CXX_AUTO_TRACE(logger_);

  const uint32_t connection_key =
      (*message)[strings::params][strings::connection_key].asUInt();
  if (0 == connection_key) {
    LOG4CXX_DEBUG(logger_,
                  "No app_id was specified. Parameters checking skipped");
    return true;
  }

  const ApplicationSharedPtr app =
      application_manager_.application(connection_key);
  if (!app) {
    LOG4CXX_ERROR(logger_,
                  "There is no registered application with "
                  "connection key '"
                      << connection_key << "'");
    return false;
  }

  RPCParams params;

  const smart_objects::SmartObject& s_map = (*message)[strings::msg_params];
  if (smart_objects::SmartType_Map == s_map.getType()) {
    smart_objects::SmartMap::const_iterator iter = s_map.map_begin();
    smart_objects::SmartMap::const_iterator iter_end = s_map.map_end();

    for (; iter != iter_end; ++iter) {
      if (helpers::Compare<smart_objects::SmartType,
                           helpers::NEQ,
                           helpers::ALL>(iter->second.getType(),
                                         smart_objects::SmartType_Null,
                                         smart_objects::SmartType_Invalid)) {
        LOG4CXX_DEBUG(logger_, "Notification param: " << iter->first);
        params.insert(iter->first);
      }
    }
  }

  CommandParametersPermissions params_permissions;
  mobile_apis::Result::eType check_result =
      application_manager_.CheckPolicyPermissions(
          app,
          MessageHelper::StringifiedFunctionID(
              static_cast<mobile_api::FunctionID::eType>(function_id())),
          params,
          &params_permissions);

  // Check, if RPC is allowed by policy
  if (mobile_apis::Result::SUCCESS != check_result) {
    LOG4CXX_ERROR(logger_,
                  "Notification for app " << app->app_id()
                                          << " is disallowed by Policies.");
    return false;
  }

  // If no parameters specified in policy table, no restriction will be
  // applied for parameters
  if (params_permissions.allowed_params.empty() &&
      params_permissions.disallowed_params.empty() &&
      params_permissions.undefined_params.empty()) {
    return true;
  }

  RemoveDisallowedParameters(message, params_permissions);

  return true;
}

void CommandNotificationImpl::RemoveDisallowedParameters(
    const MessageSharedPtr message,
    const CommandParametersPermissions& param_permissions) const {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject& params = (*message)[strings::msg_params];

  // Remove from request all disallowed parameters
  ParamsDeleter deleter_user(params,
                             "Following parameter is disallowed by user: ");
  std::for_each(param_permissions.disallowed_params.begin(),
                param_permissions.disallowed_params.end(),
                deleter_user);

  // Remove from request all undefined yet parameters
  ParamsDeleter deleter_policy(params,
                               "Following parameter is disallowed by policy: ");
  std::for_each(param_permissions.undefined_params.begin(),
                param_permissions.undefined_params.end(),
                deleter_policy);
}

void CommandNotificationImpl::Run() {}

void CommandNotificationImpl::SendNotification() {
  MessageSharedPtr message =
      utils::MakeShared<smart_objects::SmartObject>(*message_);
  (*message)[strings::params][strings::protocol_type] = mobile_protocol_type_;
  (*message)[strings::params][strings::protocol_version] = protocol_version_;
  (*message)[strings::params][strings::message_type] =
      static_cast<int32_t>(application_manager::MessageType::kNotification);

  LOG4CXX_INFO(logger_, "SendNotification");
  MessageHelper::PrintSmartObject(*message);

  if (CheckAllowedParameters(message)) {
    application_manager_.SendMessageToMobile(message);
  }
}

}  // namespace commands

}  // namespace application_manager
