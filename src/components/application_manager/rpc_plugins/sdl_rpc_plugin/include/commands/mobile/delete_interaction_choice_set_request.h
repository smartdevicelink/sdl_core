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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_DELETE_INTERACTION_CHOICE_SET_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_DELETE_INTERACTION_CHOICE_SET_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/application.h"
#include "utils/macro.h"

namespace application_manager {

namespace commands {

/**
 * @brief DeleteInteractionChoiceSetRequest command class
 **/
class DeleteInteractionChoiceSetRequest : public CommandRequestImpl {
 public:
  /**
   * @brief DeleteInteractionChoiceSetRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  DeleteInteractionChoiceSetRequest(const MessageSharedPtr& message,
                                    ApplicationManager& application_manager);

  /**
   * @brief DeleteInteractionChoiceSetRequest class destructor
   **/
  ~DeleteInteractionChoiceSetRequest();

  /**
   * @brief Execute command
   **/
  void Run() FINAL;

  /**
   * @brief Init sets hash update mode for request
   */
  bool Init() FINAL;

 private:
  /*
   * @brief Check if requested choice set ID in use by perform interaction
   *
   * @param app mobile application
   */
  bool ChoiceSetInUse(ApplicationConstSharedPtr app);

  void SendVrDeleteCommand(ApplicationSharedPtr app);

  DISALLOW_COPY_AND_ASSIGN(DeleteInteractionChoiceSetRequest);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_DELETE_INTERACTION_CHOICE_SET_REQUEST_H_
