/**
 * \file appMain.cpp
 * \brief SmartDeviceLink main application sources
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

#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>  // cpplint: Streams are highly discouraged.
#include <fstream>   // cpplint: Streams are highly discouraged.

// ----------------------------------------------------------------------------

#include "./appMain.h"
#include "./life_cycle.h"

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/signals.h"
#include "config_profile/profile.h"

// ----------------------------------------------------------------------------
// Third-Party includes
#include "networking.h"  // cpplint: Include the directory when naming .h files

// ----------------------------------------------------------------------------

namespace {

const char kBrowser[] = "/usr/bin/chromium-browser";
const char kBrowserName[] = "chromium-browser";
const char kBrowserParams[] = "--auth-schemes=basic,digest,ntlm";

/**
 * Initialize HTML based HMI.
 * @return true if success otherwise false.
 */
bool InitHmi() {
log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
                              log4cxx::Logger::getLogger("appMain"));

pid_t pid_hmi = 0;
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
int length = file_str.tellg();
file_str.seekg(0, std::ios::beg);

char* raw_data = new char[length + 1];
if (!raw_data) {
  LOG4CXX_INFO(logger, "Memory allocation failed.");
  return false;
}

memset(raw_data, 0, length + 1);
file_str.getline(raw_data, length + 1);
std::string hmi_link = std::string(raw_data, strlen(raw_data));
delete[] raw_data;

LOG4CXX_INFO(logger,
             "Input string:" << hmi_link << " length = " << hmi_link.size());
file_str.close();

if (stat(hmi_link.c_str(), &sb) == -1) {
  LOG4CXX_INFO(logger, "HMI index.html doesn't exist!");
  return false;
}
// Create a child process.
pid_hmi = fork();

switch (pid_hmi) {
  case -1: {  // Error
    LOG4CXX_INFO(logger, "fork() failed!");
    return false;
  }
  case 0: {  // Child process
    int fd_dev0 = open("/dev/null", O_RDWR, S_IWRITE);
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
    execlp(kBrowser, kBrowserName, kBrowserParams, hmi_link.c_str(),
           reinterpret_cast<char*>(0));
    LOG4CXX_WARN(logger, "execl() failed! Install chromium-browser!");

    return true;
  }
  default: { /* Parent process */
    LOG4CXX_INFO(logger, "Process created with pid " << pid_hmi);
    return true;
  }
}
}
}

/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char** argv) {

  // --------------------------------------------------------------------------
  // Logger initialization

  log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
                                log4cxx::Logger::getLogger("appMain"));
  log4cxx::PropertyConfigurator::configure("log4cxx.properties");

  LOG4CXX_INFO(logger, " Application started!");

  // Initialize gstreamer. Needed to activate debug from the command line.
  gst_init(&argc, &argv);

  // --------------------------------------------------------------------------
  // Components initialization

  profile::Profile::instance()->config_file_name("smartDeviceLink.ini");

  main_namespace::LifeCycle::instance()->StartComponents();

  // --------------------------------------------------------------------------
  // Third-Party components initialization.

  if (!main_namespace::LifeCycle::instance()->InitMessageBroker()) {
    exit(EXIT_FAILURE);
  }
  LOG4CXX_INFO(logger, "InitMessageBroker successful");

  if (!InitHmi()) {
    exit(EXIT_FAILURE);
  }
  LOG4CXX_INFO(logger, "InitHmi successful");
  // --------------------------------------------------------------------------

  utils::SubscribeToTerminateSignal(
    &main_namespace::LifeCycle::StopComponents);

  while (true) {
    sleep(100500);
  }
}

