/**
 * Copyright (c) 2013, Ford Motor Company
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

#include "rpc/init_hmi.h"
#include "utils/logger.h"
namespace {
  GETLOGGER(logger_, "MediaAdapterImpl")
}  // namespace

namespace InitializeHMI {
  #ifdef __QNX__
  bool Execute(std::string command, const char * const *) {
    if (system(command.c_str()) == -1) {
      LOG4CXX_INFO(logger, "Can't start HMI!");
      return false;
    }
    return true;
  }
  #else
  bool Execute(std::string file, const char * const * argv) {
    // Create a child process.
    pid_t pid_hmi = fork();

    switch (pid_hmi) {
      case -1: {  // Error
        LOG4CXX_INFO(logger, "fork() failed!");
        return false;
      }
      case 0: {  // Child process
        int32_t fd_dev0 = open("/dev/null", O_RDWR, S_IWRITE);
        if (0 > fd_dev0) {
          LOG4CXX_WARN(logger, "Open dev0 failed!");
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

        // Execute the program.
        if (execvp(file.c_str(), const_cast<char* const *>(argv)) == -1) {
          LOG4CXX_ERROR_WITH_ERRNO(logger, "execvp() failed! Can't start HMI!");
          _exit(EXIT_FAILURE);
        }

        return true;
      }
      default: { /* Parent process */
        LOG4CXX_INFO(logger, "Process created with pid " << pid_hmi);
        return true;
      }
    }
  }
  #endif
  #ifdef WEB_HMI
  /**
   * Initialize HTML based HMI.
   * @return true if success otherwise false.
   */
  bool InitHmi() {
    struct stat sb;
    if (stat("../src/appMain/hmi_link", &sb) == -1) {
      LOG4CXX_INFO(logger, "File with HMI link doesn't exist!");
      return false;
    }

    std::ifstream file_str;
    file_str.open("../src/appMain/hmi_link");

    if (!file_str.is_open()) {
      LOG4CXX_INFO(logger, "File with HMI link was not opened!");
      return false;
    }

    file_str.seekg(0, std::ios::end);
    int32_t length = file_str.tellg();
    file_str.seekg(0, std::ios::beg);

    std::string hmi_link;
    std::getline(file_str, hmi_link);

    LOG4CXX_INFO(logger,
                 "Input string:" << hmi_link << " length = "
                 << hmi_link.size());
    file_str.close();

    if (stat(hmi_link.c_str(), &sb) == -1) {
      LOG4CXX_INFO(logger, "HMI index.html doesn't exist!");
      return false;
    }

    std::string kBin = kBrowser;
    const char* const kParams[4] = {kBrowserName, kBrowserParams,
        hmi_link.c_str(), NULL};

    return Execute(kBin, kParams);
  }
  #endif  // WEB_HMI
  #ifdef QT_HMI
  /**
   * Initialize HTML based HMI.
   * @return true if success otherwise false.
   */
  bool InitHmi() {
    std::string kStartHmi = "./start_hmi.sh";
    struct stat sb;
    if (stat(kStartHmi.c_str(), &sb) == -1) {
      LOG4CXX_INFO(logger, "HMI start script doesn't exist!");
      return false;
    }

    return Execute(kStartHmi, NULL);
  }
  #endif  // QT_HMI

  InitHMI::InitHMI()
    : ThreadDelegate() {
  }

  InitHMI::~InitHMI() {
  }

  void InitHMI::operator=(const InitHMI&) {
  }

  InitHMI::InitHMI(const InitHMI&)
    : ThreadDelegate() {
  }

  void InitHMI::threadMain() {
    if (!InitHmi()) {
      // main_namespace::LifeCycle::instance()->StopComponents();
      // without this line log4cxx threads continue using some instances
      // destroyed by exit()
      // log4cxx::Logger::getRootLogger()->closeNestedAppenders();
      exit(EXIT_FAILURE);
    }
  }
}  // namespace InitializeHMI

