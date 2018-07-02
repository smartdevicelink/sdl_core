/*
 * Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HELP_PROMPT_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HELP_PROMPT_MANAGER_H_

#include "smart_objects/smart_object.h"

namespace application_manager {
/**
 * @brief The HelpPromptManager is a mechanism for handling "help" requests
 * by the user when the application does not provide any or such information.
 */
class HelpPromptManager {
 public:
  /**
   * @brief Class destructor
   */
  virtual ~HelpPromptManager() {}
  /**
   * @brief Adds command to constructed values, and sends SetGlobalProperties if
   * required
   */
  virtual void OnVrCommandAdded(const uint32_t cmd_id,
                                const smart_objects::SmartObject& command) = 0;
  /**
   * @brief Removes command from constructed values, and send
   * SetGlobalProperties if required
   */
  virtual void OnVrCommandDeleted(const uint32_t cmd_id) = 0;
  /**
   * @brief Stop constructing vrHelp and/or helpPrompt if they are present in
   * message
   * @param msg containing GlobalProperties
   * @param is_response determines is the request or response for the for
   * the SetGlobalPropertiesRequest
   */
  virtual void OnSetGlobalPropertiesReceived(
      const smart_objects::SmartObject& msg, const bool is_response) = 0;
  /**
   * @brief Starts mechanism for handling "help" requests
   * @param is_restore determines the need to restore commands
   * after resuming
   */
  virtual void OnAppActivated(const bool is_restore) = 0;
  /**
   * @brief Stop mechanism for handling "help" requests
   */
  virtual void OnAppUnregistered() = 0;
};

}  //  namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HELP_PROMPT_MANAGER_H_
