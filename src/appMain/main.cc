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
#include "utils/threads/thread_counter.h"

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
#ifdef THREAD_COUNT
  threads::ThreadCounter thread_counter;
#endif
  threads::Thread::SetNameForId(threads::Thread::CurrentId(), "MainThread");

  logger::LoggerAutoPtr logger_auto_ptr(profile_instance.logs_enabled());
  SDL_CREATE_LOGGER("SDLMain");

  SDL_INFO("Application started!");
  SDL_INFO("SDL version: " << profile_instance.sdl_version());

  main_namespace::LifeCycle life_cycle(profile_instance);
  if (!life_cycle.StartComponents()) {
    SDL_FATAL("Failed to start components");
    life_cycle.StopComponents();
    exit(EXIT_FAILURE);
  }
  SDL_INFO("Components have been started");

  if (!life_cycle.InitMessageSystem()) {
    SDL_FATAL("Failed to init message system");
    life_cycle.StopComponents();
    exit(EXIT_FAILURE);
  }
  SDL_INFO("Message system has been initialized");

  life_cycle.Run();
  SDL_INFO("Stop SDL due to caught signal");

  life_cycle.StopComponents();
  SDL_INFO("Application has been stopped successfuly");

  return EXIT_SUCCESS;
}
