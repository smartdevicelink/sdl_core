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

#include "application_manager/command_holder.h"

#include <string>
#include <vector>
#include "smart_objects/smart_object.h"
#include "utils/lock.h"
#include "utils/shared_ptr.h"
#include "utils/macro.h"

namespace application_manager {
class ApplicationManager;
/**
 * @brief The CommandHolderImpl class should hold HMI commands for particular
 * application specified by its policy id during application transport switching
 * process and sends for processing after switching is completed successfully
 * or drop otherwise
 */
class CommandHolderImpl : public CommandHolder {
 public:
  /**
   * @brief CommandHolderImpl constructor
   */

  explicit CommandHolderImpl(ApplicationManager& app_manager);

  /**
   * @brief Hold collects command for specific application id internally
   * @param policy_app_id Policy id of application
   * @param command Command
   */
  void Hold(const std::string& policy_app_id,
            smart_objects::SmartObjectSPtr command) FINAL;

  /**
   * @brief Release sends all collected HMI commands to ApplicationManager
   * for further processing
   * @param policy_app_id Policy id of application
   */
  void Release(const std::string& policy_app_id) FINAL;

  /**
   * @brief Drop removes all commands collected for specific application id
   * @param policy_app_id Policy application id
   */
  void Drop(const std::string& policy_app_id) FINAL;

 private:
  using AppCommands =
      std::map<std::string,
               std::vector<utils::SharedPtr<smart_objects::SmartObject> > >;

  ApplicationManager& app_manager_;
  sync_primitives::Lock commands_lock_;
  AppCommands app_commands_;
};
}  // application_manager
