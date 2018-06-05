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
#include <boost/process/io.hpp>
#include <boost/process/spawn.hpp>
#include <boost/process/system.hpp>

#include "utils/logger.h"
#include "utils/system.h"

namespace bp = boost::process;  // easier access

namespace utils {

CREATE_LOGGERPTR_LOCAL(logger_, "Utils")

System::System(const std::string& command) : command_(command) {
  argv_.push_back(command);
}

System::System(const std::string& file, const std::string& command)
    : command_(file) {
  argv_.push_back(command);
}

System& System::Add(const std::string& arg) {
  argv_.push_back(arg);
  return *this;
}

std::string System::command() const {
  return command_;
}

std::vector<std::string> System::argv() const {
  return argv_;
}

bool System::Execute() {
  return Execute(false);
}

bool System::Execute(bool wait) {
  // Empty command does nothing
  if (command_.empty()) {
    if (wait) {
      // we couldn't wait for the command because it didn't exists
      return false;
    } else {
      // we don't care if it ran, so return true
      // This is to maintain compatibility with pre-boost API
      return true;
    }
  }

  // for some reason boost.process uses STL error codes
  std::error_code ec;
  // launch new process with no I/O, can optionally wait
  bp::child c(command_,
              argv_,
              bp::std_out > bp::null,
              bp::std_in<bp::null, bp::std_err> bp::null,
              ec);

  // Catch errors
  if (ec) {
    LOG4CXX_ERROR(logger_,
                  "Can't execute command: " << command_ << " Error message is: "
                                            << ec.message());
    return false;
  }

  if (wait) {
    c.wait(ec);  // wait for the process to exit
    if (ec) {
      LOG4CXX_ERROR(logger_,
                    "Failed to wait for command: "
                        << command_ << " Error message is: " << ec.message());
      return false;
    }
    // did the child run successfully and exit cleanly?
    return c.exit_code() == 0;
  }

  // if we got this far everything went well
  return true;
}

}  // namespace utils
