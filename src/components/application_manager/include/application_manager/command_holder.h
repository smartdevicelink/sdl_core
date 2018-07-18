/*
 * Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMAND_HOLDER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMAND_HOLDER_H_

#include <string>
#include "application_manager/application.h"
#include "smart_objects/smart_object.h"


namespace application_manager {
/**
 * @brief The CommandHolder class should hold commands for particular
 * application until certain event happens
 */
class CommandHolder {
 public:
  /**
   * @brief The CommandType enum defines type of command to suspend or resume
   */
  enum class CommandType { kHmiCommand, kMobileCommand };

  /**
   * @brief ~CommandsHolder destructor
   */
  virtual ~CommandHolder() {}

  /**
   * @brief Suspend collects command for specific application policy id
   * internally
   * @param application Application pointer
   * @param type Command type
   * @param command Command
   */
  virtual void Suspend(ApplicationSharedPtr application,
                       CommandType type,
                       smart_objects::SmartObjectSPtr command) = 0;

  /**
   * @brief Resume send all collected commands for further processing and
   * removes them afterward
   * @param application Application pointer
   * @param type Command type
   */
  virtual void Resume(ApplicationSharedPtr application, CommandType type) = 0;

  /**
   * @brief Clear removes all collected commands w/o processing
   * @param application Application pointer
   */
  virtual void Clear(ApplicationSharedPtr application) = 0;
};
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMAND_HOLDER_H_
