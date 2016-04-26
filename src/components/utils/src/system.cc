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
#ifdef __QNX__
#include <process.h>
#else  // __QNX__
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif  // __QNX__

#include <algorithm>
#include <functional>
#include <cstring>
#include <iostream>

#include "utils/logger.h"
#include "utils/system.h"

namespace utils {

CREATE_LOGGERPTR_LOCAL(logger_, "Utils")

struct GetCString {
  char* operator()(const std::string& string) {
    return const_cast<char*>(string.c_str());
  }
};

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

#ifdef __QNX__

bool System::Execute(bool wait) {
  size_t size = argv_.size();
  char** argv = new char* [size + 1];
  std::transform(argv_.begin(), argv_.end(), argv, GetCString());
  argv[size] = NULL;

  int mode = wait ? P_WAIT : P_NOWAIT;
  int ret = spawnvp(mode, command_.c_str(), argv);
  delete[] argv;

  if (ret == -1) {
    LOG4CXX_ERROR(logger_,
                  "Can't execute command: " << command_ << " Errno is: "
                                            << std::strerror(errno));
    return false;
  }

  if (wait) {
    return WEXITSTATUS(ret) == 0;
  }

  return true;
}

#else  // __QNX__

bool System::Execute(bool wait) {
  // Create a child process.
  pid_t pid_command = fork();

  switch (pid_command) {
    case -1: {  // Error
      LOG4CXX_FATAL(logger_, "fork() failed!");
      return false;
    }
    case 0: {  // Child process
      int32_t fd_dev0 = open("/dev/null", O_RDWR, S_IWRITE);
      if (0 > fd_dev0) {
        LOG4CXX_FATAL(logger_, "Open dev0 failed!");
        return false;
      }
      // close input/output file descriptors.
      close(STDIN_FILENO);
      close(STDOUT_FILENO);
      close(STDERR_FILENO);

      // move input/output to /dev/null.
      dup2(fd_dev0, STDIN_FILENO);
      dup2(fd_dev0, STDOUT_FILENO);
      dup2(fd_dev0, STDERR_FILENO);

      size_t size = argv_.size();
      char** argv = new char* [size + 1];
      std::transform(argv_.begin(), argv_.end(), argv, GetCString());
      argv[size] = NULL;

      // Execute the program.
      if (execvp(command_.c_str(), argv) == -1) {
        LOG4CXX_ERROR(logger_, "Can't execute command: " << command_);
        _exit(EXIT_FAILURE);
      }
      delete[] argv;

      return true;
    }
    default: { /* Parent process */
      LOG4CXX_INFO(logger_, "Process created with pid " << pid_command);
      if (wait) {
        int status;
        pid_t wait_pid;
        do {
          wait_pid = waitpid(pid_command, &status, WUNTRACED | WCONTINUED);
          if (wait_pid == -1) {
            LOG4CXX_ERROR_WITH_ERRNO(logger_, "Can't wait");
            _exit(EXIT_FAILURE);
            return false;
          }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        return WEXITSTATUS(status) == 0;
      }

      return true;
    }
  }
}

#endif  // __QNX__

}  // utils
