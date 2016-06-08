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

#include "utils/macro.h"
#include "utils/logger.h"
#include "config_profile/profile.h"
#include "life_cycle.h"
#include "utils/appenders_loader.h"

/**
 * \brief Entry point of the program.
 * \param argc number of arguments
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char* argv[]) {
  PLATFORM_INIT(argc, argv);

  profile::Profile profile_instance;
  if ((argc > 1) && (0 != argv)) {
    profile_instance.config_file_name(argv[1]);
  } else {
    profile_instance.config_file_name("smartDeviceLink.ini");
  }

  threads::Thread::SetNameForId(threads::Thread::CurrentId(), "MainThread");

  INIT_LOGGER(profile_instance.logs_enabled());
  CREATE_LOGGERPTR_LOCAL(logger_, "SDLMain")

  LOGGER_INFO(logger_, "Application started!");
  LOGGER_INFO(logger_, "SDL version: " << profile_instance.sdl_version());

  main_namespace::LifeCycle life_cycle(profile_instance);
  if (!life_cycle.StartComponents()) {
    LOGGER_FATAL(logger_, "Failed to start components");
    life_cycle.StopComponents();
    DEINIT_LOGGER();
    exit(EXIT_FAILURE);
  }
  LOGGER_INFO(logger_, "Components have been started");

  if (!life_cycle.InitMessageSystem()) {
    LOGGER_FATAL(logger_, "Failed to init message system");
    life_cycle.StopComponents();
    DEINIT_LOGGER();
    exit(EXIT_FAILURE);
  }
  LOGGER_INFO(logger_, "Message system has been initialized");

  life_cycle.Run();
  LOGGER_INFO(logger_, "Stop SDL due to caught signal");

  life_cycle.StopComponents();
  LOGGER_INFO(logger_, "Application has been stopped successfuly");

  DEINIT_LOGGER();
  return EXIT_SUCCESS;
}
