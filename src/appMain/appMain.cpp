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

#include "utils/macro.h"
#include "utils/logger.h"
#include "config_profile/profile.h"

#include "mobile_message_handler/mobile_message_handler_impl.h"
#include "application_manager/application_manager_impl.h"
#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "connection_handler/connection_handler_impl.h"
#include "protocol_handler/protocol_handler_impl.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_default.h"
#ifdef QT_HMI
#  include "hmi_message_handler/dbus_message_adapter.h"
#endif  // QT_HMI
#ifdef WEB_HMI
#  include "hmi_message_handler/messagebroker_adapter.h"
#endif  // WEB_HMI
// ----------------------------------------------------------------------------
// Third-Party includes

#ifdef WEB_HMI
#  include "CMessageBroker.hpp"
#  include "mb_tcpserver.hpp"
#  include "networking.h"  // cpplint: Include the directory when naming .h files
#endif  // WEB_HMI
#include "system.h"      // cpplint: Include the directory when naming .h files
// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" void __gcov_flush();
#endif

namespace {

bool Execute(std::string file, const char * const * argv) {
  log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("appMain"));
  // Create a child process.
  pid_t pid_hmi = fork();

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

#ifdef WEB_HMI

/**
 * Initialize MessageBroker component
 * @return true if success otherwise false.
 */
bool InitMessageSystem() {  // TODO(AK): check memory allocation here.
  log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("appMain"));

  NsMessageBroker::CMessageBroker* message_broker =
  NsMessageBroker::CMessageBroker::getInstance();
  if (!message_broker) {
    LOG4CXX_INFO(logger, " Wrong pMessageBroker pointer!");
    return false;
  }

  NsMessageBroker::TcpServer* message_broker_server =
  new NsMessageBroker::TcpServer(
      profile::Profile::instance()->server_address(),
      profile::Profile::instance()->server_port(),
      message_broker);
  if (!message_broker_server) {
    LOG4CXX_INFO(logger, " Wrong pJSONRPC20Server pointer!");
    return false;
  }
  message_broker->startMessageBroker(message_broker_server);
  if (!networking::init()) {
    LOG4CXX_INFO(logger, " Networking initialization failed!");
    return false;
  }

  if (!message_broker_server->Bind()) {
    LOG4CXX_FATAL(logger, "Bind failed!");
    return false;
  } else {
    LOG4CXX_INFO(logger, "Bind successful!");
  }

  if (!message_broker_server->Listen()) {
    LOG4CXX_FATAL(logger, "Listen failed!");
    return false;
  } else {
    LOG4CXX_INFO(logger, " Listen successful!");
  }

  hmi_message_handler::MessageBrokerAdapter* mb_adapter =
  new hmi_message_handler::MessageBrokerAdapter(
      hmi_message_handler::HMIMessageHandlerImpl::instance());

  hmi_message_handler::HMIMessageHandlerImpl::instance()->AddHMIMessageAdapter(
      mb_adapter);
  if (!mb_adapter->Connect()) {
    LOG4CXX_INFO(logger, "Cannot connect to remote peer!");
    return false;
  }

  LOG4CXX_INFO(logger, "Start CMessageBroker thread!");
  System::Thread* th1 = new System::Thread(
      new System::ThreadArgImpl<NsMessageBroker::CMessageBroker>(
          *message_broker, &NsMessageBroker::CMessageBroker::MethodForThread,
          NULL));
  th1->Start(false);

  LOG4CXX_INFO(logger, "Start MessageBroker TCP server thread!");
  System::Thread* th2 = new System::Thread(
      new System::ThreadArgImpl<NsMessageBroker::TcpServer>(
          *message_broker_server, &NsMessageBroker::TcpServer::MethodForThread,
          NULL));
  th2->Start(false);

  LOG4CXX_INFO(logger, "StartAppMgr JSONRPC 2.0 controller receiver thread!");
  System::Thread* th3 = new System::Thread(
      new System::ThreadArgImpl<hmi_message_handler::MessageBrokerAdapter>(
          *mb_adapter,
          &hmi_message_handler::MessageBrokerAdapter::MethodForReceiverThread,
          NULL));
  th3->Start(false);

  mb_adapter->registerController();
  mb_adapter->SubscribeTo();

  return true;
}

/**
 * Initialize HTML based HMI.
 * @return true if success otherwise false.
 */
bool InitHmi() {
  log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("appMain"));

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

  std::string kBrowser = "/usr/bin/chromium-browser";
  const char* const kParams[4] = {"chromium-browser",
    "--auth-schemes=basic,digest,ntlm", hmi_link.c_str(), NULL};

  return Execute(kBrowser, kParams);
}
#endif  // WEB_HMI
#ifdef QT_HMI
/**
 * Initialize DBus component
 * @return true if success otherwise false.
 */
bool InitMessageSystem() {
  log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("appMain"));

  hmi_message_handler::DBusMessageAdapter* adapter =
      new hmi_message_handler::DBusMessageAdapter(
          hmi_message_handler::HMIMessageHandlerImpl::instance());

  hmi_message_handler::HMIMessageHandlerImpl::instance()->AddHMIMessageAdapter(
      adapter);
  if (!adapter->Init()) {
    LOG4CXX_INFO(logger, "Cannot init DBus service!");
    return false;
  }

  adapter->SubscribeTo();

  LOG4CXX_INFO(logger, "Start DBusMessageAdapter thread!");
  System::Thread* th1 = new System::Thread(
      new System::ThreadArgImpl<hmi_message_handler::DBusMessageAdapter>(
          *adapter,
          &hmi_message_handler::DBusMessageAdapter::MethodForReceiverThread,
          NULL));
  th1->Start(false);

  return true;
}

/**
 * Initialize HTML based HMI.
 * @return true if success otherwise false.
 */
bool InitHmi() {
  log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("appMain"));
  std::string kStartHmi = "./start_hmi.sh";
  struct stat sb;
  if (stat(kStartHmi.c_str(), &sb) == -1) {
    LOG4CXX_INFO(logger, "HMI start script doesn't exist!");
    return false;
  }

  return Execute(kStartHmi, NULL);
}
#endif  // QT_HMI
}  // namespace

void flushCoverageInfo() {
  log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("appMain"));
  LOG4CXX_INFO(logger, "Flush code coverage info");
#ifdef __cplusplus
  __gcov_flush();
#endif
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

  bool shouldReturn = false;
  bool shouldFlush = false;
  int next_option;

  const char* const short_options = "hf";
  const struct option long_options[] = { { "help", 0, NULL, 'h' }, { "flush", 0,
  NULL, 'f' }, { NULL, 0, NULL, 0 } };

  do {
    next_option = getopt_long(argc, argv, short_options, long_options, NULL);

    switch (next_option) {
      case 'h':
        LOG4CXX_INFO(logger, "-h or --help")
        ;
        shouldReturn = true;
        break;
      case 'f':
        LOG4CXX_INFO(logger, "-f or --flush flag")
        ;
        // -f or --flush flag
        shouldFlush = true;
        break;
      case '?':
        LOG4CXX_INFO(logger, "Wrong input")
        ;
        shouldReturn = true;
        break;
      case -1:
        LOG4CXX_INFO(logger, "No more options")
        ;
        break;
      default:
        break;
    }
  } while (next_option != -1);

  // Check shouldReturn fist
  if (shouldReturn) {
    return 0;
  }

  if (shouldFlush) {
    flushCoverageInfo();
    return 0;
  }

  LOG4CXX_INFO(logger, " Application started!");

  // --------------------------------------------------------------------------
  // Components initialization

  profile::Profile::instance()->config_file_name("smartDeviceLink.ini");

  ::transport_manager::TransportManager* transport_manager =
      ::transport_manager::TransportManagerDefault::Instance();
  DCHECK(transport_manager);

  protocol_handler::ProtocolHandlerImpl* protocol_handler =
      new protocol_handler::ProtocolHandlerImpl(transport_manager);
  DCHECK(protocol_handler);

  mobile_message_handler::MobileMessageHandlerImpl* mmh =
      mobile_message_handler::MobileMessageHandlerImpl::instance();
  DCHECK(mmh);

  connection_handler::ConnectionHandlerImpl* connection_handler =
      connection_handler::ConnectionHandlerImpl::instance();
  DCHECK(connection_handler);

  application_manager::ApplicationManagerImpl* app_manager =
      application_manager::ApplicationManagerImpl::instance();
  DCHECK(app_manager);

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();
  DCHECK(hmi_handler)

  transport_manager->SetProtocolHandler(protocol_handler);
  transport_manager->AddEventListener(protocol_handler);
  transport_manager->AddEventListener(connection_handler);

  mmh->set_protocol_handler(protocol_handler);
  hmi_handler->set_message_observer(app_manager);

  protocol_handler->set_session_observer(connection_handler);
  protocol_handler->set_protocol_observer(mmh);

  connection_handler->set_transport_manager(transport_manager);
  connection_handler->set_connection_handler_observer(app_manager);

  app_manager->set_mobile_message_handler(mmh);
  mmh->AddMobileMessageListener(app_manager);
  app_manager->set_connection_handler(connection_handler);
  app_manager->set_hmi_message_handler(hmi_handler);

  if (!InitMessageSystem()) {
    exit(EXIT_FAILURE);
  }
  LOG4CXX_INFO(logger, "InitMessageSystem successful");

  if (!InitHmi()) {
    exit(EXIT_FAILURE);
  }
  LOG4CXX_INFO(logger, "InitHmi successful");

  while (true) {
    sleep(100500);
  }
}

