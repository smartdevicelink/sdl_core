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
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

#include "utils/logger.h"
#include "life_cycle.h"
#include "utils/system.h"
#include "utils/signals.h"
#include "config_profile/profile.h"
#include "networking.h"
#include "utils/macro.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "appMain")

namespace {

const std::string kBrowser = "/usr/bin/chromium-browser";
const std::string kBrowserName = "chromium-browser";
const std::string kBrowserParams = "--auth-schemes=basic,digest,ntlm";
const std::string kLocalHostAddress = "127.0.0.1";

#ifdef WEB_HMI
/**
 * Initialize HTML based HMI.
 * @return true if success otherwise false.
 */
bool InitHmi() {
  // AN: This functionality is disabled until utils::System will be ported
  /*std::string hmi_link = profile::Profile::instance()->link_to_web_hmi();
  struct _stat sb;
  if (_stat(hmi_link.c_str(), &sb) == -1) {
    LOGGER_FATAL(logger_, "HMI index file " << hmi_link << " doesn't exist!");
    return false;
  }*/
  return true;  // utils::System(kBrowser,
                // kBrowserName).Add(kBrowserParams).Add(hmi_link)
  //.Execute();
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
    LOGGER_FATAL(logger_, "HMI start script doesn't exist!");
    return false;
  }

  return utils::System(kStartHmi).Execute();
}
#endif  // QT_HMI

#ifdef OS_POSIX
bool UnsibscribeFromTermination() {
  // Disable some system signals receiving in thread
  // by blocking those signals
  // (system signals processes only in the main thread)
  // Mustn't block all signals!
  // See "Advanced Programming in the UNIX Environment, 3rd Edition"
  // (http://poincare.matf.bg.ac.rs/~ivana//courses/ps/sistemi_knjige/pomocno/apue.pdf,
  // "12.8. Threads and Signals".
  sigset_t signal_set;
  sigemptyset(&signal_set);
  sigaddset(&signal_set, SIGINT);
  sigaddset(&signal_set, SIGTERM);

  return !pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
}
#endif

}  // namespace

/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int32_t main(int32_t argc, char** argv) {
#ifdef OS_POSIX
  // Unsibscribe once for all threads
  if (!UnsibscribeFromTermination()) {
    // Can't use internal logger here
    exit(EXIT_FAILURE);
  }
#endif

  if ((argc > 1) && (0 != argv)) {
    profile::Profile::instance()->config_file_name(argv[1]);
  } else {
    profile::Profile::instance()->config_file_name("smartDeviceLink.ini");
  }

  PLATFORM_INIT(argc, argv);
  // Logger initialization
  INIT_LOGGER(profile::Profile::instance()->logs_enabled());

  threads::Thread::SetNameForId(threads::Thread::CurrentId(), "MainThread");

  /*if (!utils::appenders_loader.Loaded()) {
    LOGGER_ERROR(logger_, "Appenders plugin not loaded, file logging
  disabled");
  }*/

  LOGGER_INFO(logger_, "Application started!");
  LOGGER_INFO(logger_,
              "SDL version: " << profile::Profile::instance()->sdl_version());

  if (!main_namespace::LifeCycle::instance()->StartComponents()) {
    LOGGER_FATAL(logger_, "Failed to start components");
    main_namespace::LifeCycle::instance()->StopComponents();
    DEINIT_LOGGER();
    exit(EXIT_FAILURE);
  }

  if (!main_namespace::LifeCycle::instance()->InitMessageSystem()) {
    LOGGER_FATAL(logger_, "Failed to init message system");
    main_namespace::LifeCycle::instance()->StopComponents();
    DEINIT_LOGGER();
    exit(EXIT_FAILURE);
  }
  LOGGER_INFO(logger_, "InitMessageBroker successful");

  if (profile::Profile::instance()->launch_hmi()) {
    if (profile::Profile::instance()->server_address() == kLocalHostAddress) {
      LOGGER_INFO(logger_, "Start HMI on localhost");

#ifndef NO_HMI
      if (!InitHmi()) {
        LOGGER_INFO(logger_, "InitHmi successful");
      } else {
        LOGGER_WARN(logger_, "Failed to init HMI");
      }
#endif  // #ifndef NO_HMI
    }
  }

  main_namespace::LifeCycle::instance()->Run();
  LOGGER_INFO(logger_, "Stopping application due to signal caught");

  main_namespace::LifeCycle::instance()->StopComponents();

  LOGGER_INFO(logger_, "Application successfully stopped");
  DEINIT_LOGGER();

  return EXIT_SUCCESS;
}
