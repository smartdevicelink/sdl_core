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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_RESET_GLOBAL_PROPERTIES_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_RESET_GLOBAL_PROPERTIES_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "utils/macro.h"

namespace application_manager {

class Application;

namespace commands {
//  TODO(DK): Include GlobalProperty header
/*
 * @brief Names of all global properties that should be unset.
 */
enum GlobalProperty {
  HELPPROMT = 0,
  TIMEOUTPROMT = 1,
  VRHELPTITLE = 2,
  VRHELPITEMS = 3
};

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

  private:
    /*
     * @brief Sets default value of the HELPPROMT global property
     * to the first vrCommand of each Command Menu registered in application
     *
     * @param app Registered application
     * @param is_timeout_promp Flag indicating that timeout prompt
     * should be reset
     */
    void ResetHelpPromt(Application* const app,
                        bool is_timeout_promp = false);

    /*
     * @brief  Sets default value of the TIMEOUTPROMT global property
     * to the first vrCommand of each Command Menu registered in application
     *
     * @param app Registered application
     */
    void ResetTimeoutPromt(Application* const app);

    /*
     * @brief Sets default value of the VRHELPTITLE global property
     * to the application name
     *
     * @param app Registered application
     */
    void ResetVrHelpTitle(Application* const app);

    /*
     * @brief Sets default value of the VRHELPITEMS global property
     * to value equal to registered command -1(default command “Help / Cancel”.)
     *
     * @param app Registered application
     */
    void ResetVrHelpItems(Application* const app);

    DISALLOW_COPY_AND_ASSIGN(ResetGlobalPropertiesRequest);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_RESET_GLOBAL_PROPERTIES_REQUEST_H_
