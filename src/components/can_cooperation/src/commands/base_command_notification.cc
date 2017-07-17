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

#include "can_cooperation/commands/base_command_notification.h"
#include "json/json.h"
#include "can_cooperation/can_module.h"
#include "can_cooperation/can_module_constants.h"
#include "application_manager/application_manager.h"

namespace can_cooperation {

namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "CANCooperation")

BaseCommandNotification::BaseCommandNotification(
    const application_manager::MessagePtr& message,
    CANModuleInterface& can_module)
    : Command(can_module), service_(can_module_.service()), message_(message) {
  Json::Value value;
  Json::Reader reader;
  reader.parse(message_->json_message(), value);
  if (value.isMember(json_keys::kParams)) {
    Json::FastWriter writer;
    message_->set_json_message(writer.write(value[json_keys::kParams]));
  } else {
    message_->set_json_message("");
  }
}

BaseCommandNotification::~BaseCommandNotification() {}

CANAppExtensionPtr BaseCommandNotification::GetAppExtension(
    application_manager::ApplicationSharedPtr app) const {
  if (!app) {
    return NULL;
  }

  functional_modules::ModuleID id = can_module_.GetModuleID();

  CANAppExtensionPtr can_app_extension;
  application_manager::AppExtensionPtr app_extension = app->QueryInterface(id);
  if (!app_extension) {
    return NULL;
  }

  can_app_extension = application_manager::AppExtensionPtr::static_pointer_cast<
      CANAppExtension>(app_extension);

  return can_app_extension;
}

void BaseCommandNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!service_->ValidateMessageBySchema(*message())) {
    LOG4CXX_ERROR(logger_, "Message validation failed");
    return;
  }

  LOG4CXX_INFO(logger_, "Notification message validated successfully!");

  Execute();
}

void BaseCommandNotification::NotifyApplications() {
  LOG4CXX_AUTO_TRACE(logger_);
  typedef std::vector<application_manager::ApplicationSharedPtr> AppList;
  AppList applications = service_->GetApplications(can_module_.GetModuleID());
  for (AppList::iterator i = applications.begin(); i != applications.end();
       ++i) {
    application_manager::MessagePtr message(
        new application_manager::Message(*message_));
    message->set_connection_key((*i)->app_id());
    NotifyOneApplication(message);
  }
}

void BaseCommandNotification::NotifyOneApplication(
    application_manager::MessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (CheckPolicy(message)) {
    service_->SendMessageToMobile(message);
  } else {
    LOG4CXX_WARN(logger_,
                 "Function \"" << message->function_name() << "\" (#"
                               << message->function_id()
                               << ") not allowed by policy");
  }
}

bool BaseCommandNotification::CheckPolicy(
    application_manager::MessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
  application_manager::ApplicationSharedPtr app =
      service_->GetApplication(message->connection_key());

  if (!app) {
    LOG4CXX_WARN(logger_,
                 "Application doesn't " << message->connection_key()
                                        << " registered");
    return false;
  }

  mobile_apis::Result::eType permission =
      service_->CheckPolicyPermissions(message);

  Json::Value value;
  Json::Reader reader;
  LOG4CXX_DEBUG(logger_, "Notification: " << message->json_message());
  reader.parse(message->json_message(), value);

  return permission == mobile_apis::Result::eType::SUCCESS &&
         service_->CheckModule(app->app_id(), ModuleType(value));
}

std::string BaseCommandNotification::ModuleType(const Json::Value& message) {
  return "";
}

std::vector<std::string> BaseCommandNotification::ControlData(
    const Json::Value& message) {
  return std::vector<std::string>();
}

}  // namespace commands

}  // namespace can_cooperation
