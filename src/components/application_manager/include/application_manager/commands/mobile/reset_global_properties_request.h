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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_RESET_GLOBAL_PROPERTIES_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_RESET_GLOBAL_PROPERTIES_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/application.h"
#include "utils/macro.h"

namespace application_manager {

namespace commands {

/**
 * @brief ResetGlobalPropertiesRequest command class
 **/
class ResetGlobalPropertiesRequest : public CommandRequestImpl {
 public:
  /**
   * @brief ResetGlobalPropertiesRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  explicit ResetGlobalPropertiesRequest(const MessageSharedPtr& message);

  /**
   * @brief ResetGlobalPropertiesRequest class destructor
   **/
  virtual ~ResetGlobalPropertiesRequest();

  /**
   * @brief Execute command
   **/
  virtual void Run();

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const event_engine::Event& event);

 private:
  /*
   * @brief Sets default value of the HELPPROMT global property
   * to the first vrCommand of each Command Menu registered in application
   *
   * @param app Registered application
   * @param is_timeout_promp Flag indicating that timeout prompt
   * should be reset
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool ResetHelpPromt(ApplicationSharedPtr app);

  /*
   * @brief  Sets default value of the TIMEOUTPROMT global property
   * to the first vrCommand of each Command Menu registered in application
   *
   * @param app Registered application
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool ResetTimeoutPromt(application_manager::ApplicationSharedPtr const app);

  /*
   * @brief Sets default value of the VRHELPTITLE global property
   * to the application name and value of the VRHELPITEMS global property
   * to value equal to registered command -1(default command “Help / Cancel”.)
   *
   * @param app Registered application
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool ResetVrHelpTitleItems(application_manager::ApplicationSharedPtr const app);

  /*
   * @brief Check if there some not delivered hmi responses exist
   *
   * @return true if all responses received
   */
  bool IsPendingResponseExist();

  DISALLOW_COPY_AND_ASSIGN(ResetGlobalPropertiesRequest);

  bool is_ui_send_;
  bool is_tts_send_;

  bool is_ui_received_;
  bool is_tts_received_;

  hmi_apis::Common_Result::eType ui_result_;
  hmi_apis::Common_Result::eType tts_result_;
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_RESET_GLOBAL_PROPERTIES_REQUEST_H_
