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

namespace can_cooperation {

namespace commands {

using namespace json_keys;

CREATE_LOGGERPTR_GLOBAL(logger_, "CANCooperation")

BaseCommandNotification::BaseCommandNotification(
    const application_manager::MessagePtr& message)
  : message_(message),
    need_reset_(false) {
  service_ = CANModule::instance()->service();

  Json::Value value;
  Json::Reader reader;
  reader.parse(message_->json_message(), value);
  if (value.isMember(kParams)) {
    Json::FastWriter writer;
    message_->set_json_message(writer.write(value[kParams]));
  } else {
    message_->set_json_message("");
  }
}


BaseCommandNotification::~BaseCommandNotification() {
}

application_manager::ApplicationSharedPtr BaseCommandNotification::GetApplicationWithControl(
                                        CANAppExtensionPtr& can_app_extension) {
  std::vector<application_manager::ApplicationSharedPtr> applications =
        service_->GetApplications(CANModule::instance()->GetModuleID());

  std::vector<application_manager::ApplicationSharedPtr>::iterator it =
      applications.begin();

  for (;it != applications.end(); ++it) {
    if (*it) {
      application_manager::AppExtensionPtr app_extension =
          (*it)->QueryInterface(CANModule::instance()->GetModuleID());
      if (app_extension) {
        can_app_extension = application_manager::AppExtensionPtr::
            static_pointer_cast<CANAppExtension>(app_extension);
        if (can_app_extension->IsControlGiven()) {
          return (*it);
        }
      }
    }
  }

  return application_manager::ApplicationSharedPtr();
}

void BaseCommandNotification::Run() {
  // TODO(KKolodiy): No explicit requirements to remove access if notification is received from CAN
  //if (need_reset_) {
  //  service_->RemoveAccess(message_->function_name());
  //}
  mobile_apis::Result::eType ret = service_->CheckPolicyPermissions(message_);
  if (ret == mobile_apis::Result::eType::SUCCESS) {
    Execute();  // run child's logic
  } else {
    LOG4CXX_WARN(logger_,
                 "Function \"" << message_->function_name() << "\" (#"
                 << message_->function_id() << ") not allowed by policy");
  }
}

}  // namespace commands

}  // namespace can_cooperation

