﻿/*
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_IMPL_H_

#include "application_manager/commands/command.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/application_manager.h"
#include "application_manager/smart_object_keys.h"
#include "utils/logger.h"

namespace application_manager {

/**
 * @brief Contains command parameters permissions (if any) defined in policy
 * table
 */
struct CommandParametersPermissions {
  RPCParams allowed_params;
  RPCParams disallowed_params;
  RPCParams undefined_params;
};

namespace commands {
/**
 * @brief Class is intended to encapsulate RPC as an object
 **/
class CommandImpl : public Command {
 public:
  /**
   * @brief CommandImpl class constructor
   *
   * @param message Incoming SmartObject message
   **/
  CommandImpl(const MessageSharedPtr& message,
              ApplicationManager& application_manager);

  /**
   * @brief CommandImpl class destructor
   *
   **/
  virtual ~CommandImpl();

  /**
   * @brief Checks command permissions according to policy table
   */
  bool CheckPermissions() OVERRIDE;

  /**
   * @brief Init required by command resources
   **/
  bool Init() OVERRIDE;

  /**
   * @brief Cleanup all resources used by command
   **/
  bool CleanUp() OVERRIDE;

  /**
   * @brief Execute corresponding command by calling the action on reciever
   **/
  void Run() OVERRIDE;
  /**
   * @brief Retrieves request default timeout.
   * If request has a custom timeout, request_timeout_ should be reassign to it
   *
   * @return Request default timeout
   */
  uint32_t default_timeout() const OVERRIDE;

  /*
   * @brief Retrieves correlation ID
   */
  uint32_t correlation_id() const OVERRIDE;

  /*
   * @brief Retrieves connection key
   */
  uint32_t connection_key() const OVERRIDE;

  /*
   * @brief Retrieves request ID
   */
  int32_t function_id() const OVERRIDE;

  /*
   * @brief Function is called by RequestController when request execution time
   * has exceed it's limit
   *
   */
  void onTimeOut() OVERRIDE;

  /**
 * @brief AllowedToTerminate tells request Controller if it can terminate this
 * request by response.
 * By default, RequestCtrl should terminate all requests by their responses.
 *  If request need to terminate itself, it should override this function false
 * @return allowed_to_terminate_ value
 */
  bool AllowedToTerminate() OVERRIDE;

  /**
  * @brief SetAllowedToTerminate set up allowed to terminate flag.
  * If true, request controller will terminate request on response
  */
  void SetAllowedToTerminate(const bool allowed) OVERRIDE;

  // members
  static const int32_t hmi_protocol_type_;
  static const int32_t mobile_protocol_type_;
  static const int32_t protocol_version_;

 protected:
  /**
   * @brief Parse smartObject and replace mobile app Id by HMI app ID
   *
   * @param message Smartobject to be parsed
   */
  void ReplaceMobileByHMIAppId(smart_objects::SmartObject& message);

  /**
   * @brief Parse smartObject and replace HMI app ID by mobile app Id
   *
   * @param message Smartobject to be parsed
   */
  void ReplaceHMIByMobileAppId(smart_objects::SmartObject& message);

  MessageSharedPtr message_;
  uint32_t default_timeout_;
  bool allowed_to_terminate_;
  ApplicationManager& application_manager_;

#ifdef ENABLE_LOG
  static log4cxx::LoggerPtr logger_;
#endif  // ENABLE_LOG

 private:
  DISALLOW_COPY_AND_ASSIGN(CommandImpl);
};

}  // namespace commands
}  // namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_IMPL_H_
