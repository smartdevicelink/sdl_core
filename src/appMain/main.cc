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
 *
 *
 * FILE CONTENTS
 *  InitHmi()
 *  main()
 */

#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "config_profile/profile.h"
#include "life_cycle.h"
#include "media_manager/media_manager_impl.h"
// TODO(MW): Update when cmake include paths are fixed,
//           resolves to src/3rd_party-static/MessageBroker/include/networking.h
#include "networking.h"
#include "signal_handlers.h"
#include "utils/appenders_loader.h"
#include "utils/log_message_loop_thread.h"
#include "utils/logger.h"
#include "utils/signals.h"
#include "utils/system.h"
#ifdef EXTENDED_MEDIA_MODE
# include <gst/gst.h>
#endif  // EXTNDED_MEDIA_MODE

CREATE_LOGGERPTR_GLOBAL(logger_, "appMain")
extern const char* gitVersion;

namespace {
const std::string kBrowser = "/usr/bin/chromium-browser";
const std::string kBrowserName = "chromium-browser";
const std::string kBrowserParams = "--auth-schemes=basic,digest,ntlm";
const std::string kLocalHostAddress = "127.0.0.1";
const std::string kApplicationVersion = "Develop";

// TODO(MW): These guarded method declarations should be refactored, as there is
//           the potential for name collision at compile time.
#ifdef WEB_HMI
// Initializes an HTML-based HMI. Returns true if successful.
bool InitHmi() {
  // TODO(MW): Bad variable name.
  struct stat sb;
  if (stat("hmi_link", &sb) == -1) {
    LOG4CXX_FATAL(logger_, "File with HMI link doesn't exist!");
    return false;
  }

  std::ifstream file_str;
  file_str.open("hmi_link");
  if (!file_str.is_open()) {
    LOG4CXX_FATAL(logger_, "File with HMI link was not opened!");
    return false;
  }

  std::string hmi_link;
  std::getline(file_str, hmi_link);
  LOG4CXX_INFO(logger_,
               "Input string:" << hmi_link << " length = " << hmi_link.size());
  file_str.close();

  if (stat(hmi_link.c_str(), &sb) == -1) {
    LOG4CXX_FATAL(logger_, "HMI index.html doesn't exist!");
    return false;
  }

  // TODO(MW): This should probably be refactored.
  return utils::System(kBrowser, kBrowserName).Add(kBrowserParams).Add(hmi_link)
        .Execute();
}
#endif  // WEB_HMI

#ifdef QT_HMI
// Initializes an HTML-based HMI. Returns true if successful.
bool InitHmi() {
  std::string kStartHmi = "./start_hmi.sh";
  struct stat sb;
  if (stat(kStartHmi.c_str(), &sb) == -1) {
    LOG4CXX_FATAL(logger_, "HMI start script doesn't exist!");
    return false;
  }

  return utils::System(kStartHmi).Execute();
}
#endif  // QT_HMI
}  // namespace

int32_t main(int32_t argc, char** argv) {
  // Logger initialization
  INIT_LOGGER("log4cxx.properties");
  LOG4CXX_INFO(logger_, gitVersion);
#if (defined(__QNXNTO__) && defined(GCOV_ENABLED))
  // TODO(MW): Is this a valid call?
  LOG4CXX_WARN(logger_,
                "Attention! This application was built with unsupported "
                "configuration (gcov + QNX). Use it at your own risk.");
#endif  // (defined(__QNXNTO__) && defined(GCONV_ENABLED))

  threads::Thread::SetNameForId(threads::Thread::CurrentId(), "MainThread");

  if (!utils::appenders_loader.Loaded()) {
    LOG4CXX_ERROR(logger_,
                  "Appenders plugin not loaded, file logging disabled");
  }

  LOG4CXX_INFO(logger_, "Application started!");
  LOG4CXX_INFO(logger_, "Application version " << kApplicationVersion);

  // Initialize gstreamer. Needed to activate debug from the command line.
#if defined(EXTENDED_MEDIA_MODE)
  gst_init(&argc, &argv);
#endif  // defined(EXTENDED_MEDIA_MODE)

  // Components initialization
  if (argc > 1) {
      profile::Profile::instance()->config_file_name(argv[1]);
  } else {
      profile::Profile::instance()->config_file_name("smartDeviceLink.ini");
  }

#ifdef __QNX__
  if (profile::Profile::instance()->enable_policy()) {
    if (!utils::System("./init_policy.sh").Execute(true)) {
      LOG4CXX_FATAL(logger_, "Failed to initialize policy database");
      DEINIT_LOGGER();
      // TODO(MW): This should be refactored into a return statement.
      exit(EXIT_FAILURE);
    }
  }
#endif  // __QNX__

  if (!main_namespace::LifeCycle::instance()->StartComponents()) {
    LOG4CXX_FATAL(logger_, "Failed to start components");
    main_namespace::LifeCycle::instance()->StopComponents();
    DEINIT_LOGGER();
    // TODO(MW): This should be refactored into a return statement.
    exit(EXIT_FAILURE);
  }

  // Third-Party components initialization.
  if (!main_namespace::LifeCycle::instance()->InitMessageSystem()) {
    LOG4CXX_FATAL(logger_, "Failed to init message system");
    DEINIT_LOGGER();
    // TODO(MW): This should be refactored into a return statement.
    exit(EXIT_FAILURE);
  }
  LOG4CXX_INFO(logger_, "InitMessageBroker successful");

  if (profile::Profile::instance()->launch_hmi()) {
    if (profile::Profile::instance()->server_address() == kLocalHostAddress) {
      LOG4CXX_INFO(logger_, "Start HMI on localhost");
#ifndef NO_HMI
      if (!InitHmi()) {
        LOG4CXX_FATAL(logger_, "Failed to init HMI");
        main_namespace::LifeCycle::instance()->StopComponents();
        DEINIT_LOGGER();
        // TODO(MW): This should be refactored into a return statement.
        exit(EXIT_FAILURE);
      }
      LOG4CXX_INFO(logger_, "InitHmi successful");
#endif  // #ifndef NO_HMI
    }
  }

  main_namespace::LifeCycle::instance()->Run();

  LOG4CXX_INFO(logger_, "Stopping application due to signal caught");
  main_namespace::LifeCycle::instance()->StopComponents();
  LOG4CXX_INFO(logger_, "Application successfully stopped");
  DEINIT_LOGGER();

  return EXIT_SUCCESS;
}
