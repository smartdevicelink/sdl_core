/*
 * Copyright (c) 2016, Ford Motor Company
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

#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>   // cpplint: Streams are highly discouraged.
#include <iostream>  // cpplint: Streams are highly discouraged.
#include <memory>
#include <string>
#include <vector>

// ----------------------------------------------------------------------------

#ifdef ENABLE_LOG
#include "utils/log_message_loop_thread.h"
#endif  // ENABLE_LOG
#include "utils/logger.h"

#include "appMain/life_cycle_impl.h"
#include "signal_handlers.h"

#include "config_profile/profile.h"
#include "utils/appenders_loader.h"
#include "utils/signals.h"
#include "utils/system.h"

#if defined(EXTENDED_MEDIA_MODE)
#include <gst/gst.h>
#endif

#include "media_manager/media_manager_impl.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "SDLMain")

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
bool InitHmi(std::string hmi_link) {
  struct stat sb;
  if (stat(hmi_link.c_str(), &sb) == -1) {
    LOG4CXX_FATAL(logger_, "HMI index file " << hmi_link << " doesn't exist!");
    return false;
  }
  return utils::System(kBrowser, kBrowserName)
      .Add(kBrowserParams)
      .Add(hmi_link)
      .Execute();
}
#endif  // WEB_HMI
}  // namespace

/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int32_t main(int32_t argc, char** argv) {
  // Unsubscribe once for all threads
  if (!utils::Signals::UnsubscribeFromTermination()) {
    // Can't use internal logger here
    exit(EXIT_FAILURE);
  }

  // --------------------------------------------------------------------------
  // Components initialization
  profile::Profile profile_instance;
  std::unique_ptr<main_namespace::LifeCycle> life_cycle(
      new main_namespace::LifeCycleImpl(profile_instance));

  if ((argc > 1) && (0 != argv)) {
    profile_instance.set_config_file_name(argv[1]);
  } else {
    profile_instance.set_config_file_name("smartDeviceLink.ini");
  }

  // Reading profile offsets for real-time signals dedicated
  // for Low Voltage functionality handling
  main_namespace::LowVoltageSignalsOffset signals_offset{
      profile_instance.low_voltage_signal_offset(),
      profile_instance.wake_up_signal_offset(),
      profile_instance.ignition_off_signal_offset()};

  // Unsubscribe once for all threads
  // except specific thread dedicated for
  // Low Voltage signals handling
  // Thread will be created later
  if (!utils::Signals::UnsubscribeFromLowVoltageSignals(signals_offset)) {
    // Can't use internal logger here
    exit(EXIT_FAILURE);
  }

  // --------------------------------------------------------------------------
  // Logger initialization
  INIT_LOGGER("log4cxx.properties", profile_instance.logs_enabled());

  threads::Thread::SetNameForId(threads::Thread::CurrentId(), "SDLCore");

  if (!utils::appenders_loader.Loaded()) {
    LOG4CXX_ERROR(logger_,
                  "Appenders plugin not loaded, file logging disabled");
  }

  LOG4CXX_INFO(logger_, "Application started!");
  LOG4CXX_INFO(logger_, "SDL version: " << profile_instance.sdl_version());

  // Check if no error values were read from config file
  if (profile_instance.ErrorOccured()) {
    LOG4CXX_FATAL(logger_, profile_instance.ErrorDescription());
    FLUSH_LOGGER();
    DEINIT_LOGGER();
    exit(EXIT_FAILURE);
  }

  // --------------------------------------------------------------------------
  // Components initialization
  if (!life_cycle->StartComponents()) {
    LOG4CXX_FATAL(logger_, "Failed to start components");
    life_cycle->StopComponents();
    DEINIT_LOGGER();
    exit(EXIT_FAILURE);
  }
  LOG4CXX_INFO(logger_, "Components Started");

  // --------------------------------------------------------------------------
  // Third-Party components initialization.

  if (!life_cycle->InitMessageSystem()) {
    LOG4CXX_FATAL(logger_, "Failed to init message system");
    life_cycle->StopComponents();
    DEINIT_LOGGER();
    _exit(EXIT_FAILURE);
  }
  LOG4CXX_INFO(logger_, "InitMessageBroker successful");
  if (profile_instance.launch_hmi()) {
    if (profile_instance.server_address() == kLocalHostAddress) {
      LOG4CXX_INFO(logger_, "Start HMI on localhost");

#ifdef WEB_HMI
      if (!InitHmi(profile_instance.link_to_web_hmi())) {
        LOG4CXX_INFO(logger_, "InitHmi successful");
      } else {
        LOG4CXX_WARN(logger_, "Failed to init HMI");
      }
#endif
    }
  }
  // --------------------------------------------------------------------------

  life_cycle->Run();
  LOG4CXX_INFO(logger_, "Stop SDL due to caught signal");

  life_cycle->StopComponents();
  LOG4CXX_INFO(logger_, "Application has been stopped successfuly");

  DEINIT_LOGGER();

  return EXIT_SUCCESS;
}
