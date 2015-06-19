/*
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_H_

#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/smart_object_keys.h"

namespace application_manager {
/**
 * @brief SmartObject type
 **/

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

namespace commands {

/**
 * @brief Base command class
 **/
class Command {
 public:
  /**
   * @brief Checks command permissions according to policy table
   */
  virtual bool CheckPermissions() = 0;

  /**
   * @brief Command initialization function
   **/
  virtual bool Init() = 0;

  /**
   * @brief Execute command
   **/
  virtual void Run() = 0;

  /**
   * @brief Free resources
   **/
  virtual bool CleanUp() = 0;

  /**
   * \brief Command class destructor
   **/
  virtual ~Command() {}

  /**
   * @brief Retrieves request default timeout.
   * If request has a custom timeout, request_timeout_ should be reassign to it
   *
   * @return Request default timeout
   */
  virtual uint32_t default_timeout() const = 0;

  /*
   * @brief Retrieves correlation ID
   */
  virtual uint32_t correlation_id() const = 0;

  /*
   * @brief Retrieves connection_key
   */
  virtual uint32_t connection_key() const = 0;

  /*
   * @brief Retrieves request ID
   */
  virtual int32_t function_id() const = 0;

  /*
   * @brief Function is called by RequestController when request execution time
   * has exceed it's limit
   *
   */
  virtual void onTimeOut() = 0;

  /**
 * @brief AllowedToTerminate tells if request controller is allowed
 * to terminate this command
 * @return
 */
  virtual bool AllowedToTerminate() = 0;

    /**
   * @brief SetAllowedToTerminate set up allowed to terminate flag.
   * If true, request controller will terminate request on response
   */
  virtual void SetAllowedToTerminate(bool allowed) = 0;


  enum CommandOrigin {
    ORIGIN_SDL,
    ORIGIN_MOBILE
  };
};

typedef smart_objects::SmartObjectSPtr MessageSharedPtr;

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_H_
