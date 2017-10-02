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

#pragma once

#include <string>
#include "smart_objects/smart_object.h"
#include "utils/shared_ptr.h"

namespace application_manager {
/**
 * @brief The CommandsHolder class should hold commands for particular
 * application specified by its id
 */
class CommandsHolder {
 public:
  /**
   * @brief ~CommandsHolder destructor
   */
  virtual ~CommandsHolder() {}

  /**
   * @brief Hold collects command for specific application policy id internally
   * @param policy_app_id Application policy id
   * @param command Command
   */
  virtual void Hold(const std::string& policy_app_id,
                    utils::SharedPtr<smart_objects::SmartObject> command) = 0;

  /**
   * @brief Release send all collected commands for further processing and
   * removes them afterward
   * @param policy_app_id Application policy id
   */
  virtual void Release(const std::string& policy_app_id) = 0;

  /**
   * @brief Drop removes all collected commands w/o processing
   * @param policy_app_id Application policy id
   */
  virtual void Drop(const std::string& policy_app_id) = 0;
};
}  // application_manager
