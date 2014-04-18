/**
 * \file appMain.cc
 * \brief SmartDeviceLink main application sources
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

#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>  // cpplint: Streams are highly discouraged.
#include <fstream>   // cpplint: Streams are highly discouraged.

// ----------------------------------------------------------------------------

#include "./life_cycle.h"

#include "utils/signals.h"
#include "config_profile/profile.h"

#if defined(EXTENDED_MEDIA_MODE)
#include <gst/gst.h>
#endif

#include "media_manager/media_manager_impl.h"
// ----------------------------------------------------------------------------
// Third-Party includes
#include "networking.h"  // cpplint: Include the directory when naming .h files

// ----------------------------------------------------------------------------

CREATE_LOGGER(logger, "appMain")
namespace {

const char kBrowser[] = "/usr/bin/chromium-browser";
const char kBrowserName[] = "chromium-browser";
const char kBrowserParams[] = "--auth-schemes=basic,digest,ntlm";
const char kLocalHostAddress[] = "127.0.0.1";
const char kApplicationVersion[] = "SDL_RB_3.3";

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
if (stat("hmi_link", &sb) == -1) {
  LOG4CXX_INFO(logger, "File with HMI link doesn't exist!");
  return false;
}

std::ifstream file_str;
file_str.open("hmi_link");

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
             "Input string:" << hmi_link << " length = " << hmi_link.size());
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

}

/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int32_t main(int32_t argc, char** argv) {

  // --------------------------------------------------------------------------
  // Logger initialization
  INITLOGGER("log4cxx.properties");

  threads::Thread::SetNameForId(threads::Thread::CurrentId(), "MainThread");

  LOG4CXX_INFO(logger, "Application started!");
  LOG4CXX_INFO(logger, "Application version " << kApplicationVersion);

  // Initialize gstreamer. Needed to activate debug from the command line.
#if defined(EXTENDED_MEDIA_MODE)
  gst_init(&argc, &argv);
#endif

  // --------------------------------------------------------------------------
  // Components initialization
  if ((argc > 1)&&(0 != argv)) {
      profile::Profile::instance()->config_file_name(argv[1]);
  } else {
      profile::Profile::instance()->config_file_name("smartDeviceLink.ini");
  }

  if (!main_namespace::LifeCycle::instance()->StartComponents()) {
    return 1;
  }

  // --------------------------------------------------------------------------
  // Third-Party components initialization.

  if (!main_namespace::LifeCycle::instance()->InitMessageSystem()) {
    main_namespace::LifeCycle::instance()->StopComponents();
    DEINITLOGGER();
    exit(EXIT_FAILURE);
  }
  LOG4CXX_INFO(logger, "InitMessageBroker successful");

  if (profile::Profile::instance()->launch_hmi()) {
    if (profile::Profile::instance()->server_address() ==
        std::string(kLocalHostAddress)) {
      LOG4CXX_INFO(logger, "Start HMI on localhost");

#ifndef NO_HMI
      if (!InitHmi()) {
        main_namespace::LifeCycle::instance()->StopComponents();
        DEINITLOGGER();
        exit(EXIT_FAILURE);
      }
      LOG4CXX_INFO(logger, "InitHmi successful");
#endif  // #ifndef NO_HMI
    }
  }
  // --------------------------------------------------------------------------

  utils::SubscribeToTerminateSignal(
    &main_namespace::LifeCycle::StopComponentsOnSignal);

  pause();
}
