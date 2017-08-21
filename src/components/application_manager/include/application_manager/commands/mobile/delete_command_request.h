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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_DELETE_COMMAND_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_DELETE_COMMAND_REQUEST_H_

#include <string>

#include "application_manager/commands/command_request_impl.h"
#include "utils/macro.h"

namespace application_manager {

namespace commands {

/**
 * @brief DeleteCommandRequest command class
 **/
class DeleteCommandRequest : public CommandRequestImpl {
 public:
  /**
   * @brief DeleteCommandRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  DeleteCommandRequest(const MessageSharedPtr& message,
                       ApplicationManager& application_manager);

  /**
   * @brief DeleteCommandRequest class destructor
   **/
  virtual ~DeleteCommandRequest();

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
  DISALLOW_COPY_AND_ASSIGN(DeleteCommandRequest);

  /*
   * @brief Check if there some not delivered hmi responses exist
   *
   * @return true if all responses received
   */
  bool IsPendingResponseExist();

  /*
   * @brief Prepare result code and result for sending to mobile application
   * @param result_code contains result code for sending to mobile application
   * @param info contains info for mobile app.
   * @return result for sending to mobile application.
   */
  bool PrepareResponseParameters(mobile_apis::Result::eType& result_code,
                                 std::string& info);

  bool is_ui_send_;
  bool is_vr_send_;

  bool is_ui_received_;
  bool is_vr_received_;

  hmi_apis::Common_Result::eType ui_result_;
  hmi_apis::Common_Result::eType vr_result_;
  std::string ui_info_;
  std::string vr_info_;
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_DELETE_COMMAND_REQUEST_H_
