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

#ifndef SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_COMMANDS_BASE_COMMAND_REQUEST_H_
#define SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_COMMANDS_BASE_COMMAND_REQUEST_H_

#include "can_cooperation/commands/command.h"
#include "can_cooperation/event_engine/event_observer.h"
#include "application_manager/message.h"
#include "application_manager/service.h"
#include "utils/logger.h"
#include "interfaces/HMI_API.h"
#include "can_cooperation/can_app_extension.h"
#include "json/json.h"

namespace can_cooperation {

namespace commands {

/**
 * @brief Base command class for requests
 */
class BaseCommandRequest : public Command,
public event_engine::EventObserver<application_manager::MessagePtr, std::string>{
 public:
  /**
   * @brief BaseCommandRequest class constructor
   *
   * @param message Message from mobile
   **/
  explicit BaseCommandRequest(const application_manager::MessagePtr& message);

  /**
   * @brief BaseCommandRequest class destructor
   */
  virtual ~BaseCommandRequest();

 protected:
  application_manager::MessagePtr message_;
  application_manager::ServicePtr service_;
  Json::Value response_params_;

  /**
   * @brief Get extension for specified application. If extension doesn't exist, it will be created
   * @param app pointer to application
   * @return pointer to extension
   */
  CANAppExtensionPtr GetAppExtension(
      application_manager::ApplicationSharedPtr app) const;

  /**
   * @brief Converts HMI result code to string with mobile result code
   *
   * @param hmi_code HMI result code
   * @return String with mobile result code
   */
  const char* GetMobileResultCode(
      const hmi_apis::Common_Result::eType& hmi_code) const;

  /**
   * @brief Creates Mobile response
   *
   * @param success true if successful; false, if failed
   * @param result_code Mobile result code in string ("SUCCESS", "INVALID_DATA", e.t.c)
   * @param info Provides additional human readable info regarding the result(may be empty)
   */
  void SendResponse(bool success,
                    const char* result_code,
                    const std::string& info);

  /**
   * @brief Sends request to CAN or HMI
   * @param function_id request ID
   * @param msg_params json with message params
   */
  void SendRequest(const char* function_id,
                   const Json::Value& message_params,
                   bool is_hmi_request = false);
};

}  // namespace commands

}  // namespace can_cooperation

#endif  // SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_COMMANDS_BASE_COMMAND_REQUEST_H_
