/*
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SYSTEM_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SYSTEM_H_

#include <string>
#include <vector>

namespace utils {

/**
 * Class to execute shell scripts
 */
class System {
 public:
  /**
   * Constructs instantiation
   * @param command name of command for executing
   */
  explicit System(const std::string& command);

  /**
   * Constructs instantiation
   * @param file name of file for executing
   * @param command name of command
   */
  System(const std::string& file, const std::string& command);

  /**
   * Adds argument
   * @param arg argument of command
   * @return itself object
   */
  System& Add(const std::string& arg);

  /**
   * Executes command as new child process
   * @return true if success
   */
  bool Execute();

  /**
   * Executes command
   * @param wait if this flag is true then wait until command is terminated
   * @return true if success
   */
  bool Execute(bool wait);

  std::string command() const;
  std::vector<std::string> argv() const;

 private:
  /**
   * Command for executing
   */
  std::string command_;

  /**
   * List of arguments
   */
  std::vector<std::string> argv_;
};

}  // utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SYSTEM_H_
