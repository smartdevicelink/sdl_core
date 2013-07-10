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

#include "appMain.h"

#ifndef BUILD_ONLY_TM
#include "protocol_handler/protocol_handler_impl.h"

#include "connection_handler/connection_handler_impl.h"
#include "mobile_message_handler/mobile_message_handler_impl.h"
#include "utils/macro.h"

/*#include "AppMgr/AppMgr.h"
 #include "AppMgr/AppMgrCore.h"*/
#include "application_manager/application_manager_impl.h"
#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "hmi_message_handler/messagebroker_adapter.h"

#include "CMessageBroker.hpp"

#include "mb_tcpserver.hpp"
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
#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "hmi_message_handler/messagebroker_adapter.h"
#include "application_manager/application_manager_impl.h"
#include "connection_handler/connection_handler_impl.h"
#include "protocol_handler/protocol_handler_impl.h"
#include "TransportManager/ITransportManager.hpp"
#include "TransportManager/ITransportManagerDeviceListener.hpp"
#endif
// ----------------------------------------------------------------------------
// Third-Party includes
/*class CTransportManagerListener : public NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener
 {
 public:

 CTransportManagerListener(NsSmartDeviceLink::NsTransportManager::ITransportManager * transportManager);


 #include "CMessageBroker.hpp"
 #include "mb_tcpserver.hpp"
 #include "networking.h"  // cpplint: Include the directory when naming .h files
 #include "system.h"      // cpplint: Include the directory when naming .h files

 // ----------------------------------------------------------------------------

 namespace {

 const char kBrowser[] = "/usr/bin/chromium-browser";
 const char kBrowserName[] = "chromium-browser";
 const char kBrowserParams[] = "--auth-schemes=basic,digest,ntlm";

 /**
 * Initialize MessageBroker component
 * @return true if success otherwise false.
 */
/**********************************/
/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */

int main(int argc, char** argv) {
#ifndef BUILD_ONLY_TM
  // --------------------------------------------------------------------------
  // Logger initialization

  log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("appMain"));
  log4cxx::PropertyConfigurator::configure("log4cxx.properties");
  LOG4CXX_INFO(logger, " Application started!");

  // --------------------------------------------------------------------------
  // Components initialization

  profile::Profile::instance()->config_file_name("smartDeviceLink.ini");

  NsSmartDeviceLink::NsTransportManager::ITransportManager* transport_manager =
  NsSmartDeviceLink::NsTransportManager::ITransportManager::create();
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

  transport_manager->addDataListener(protocol_handler);
  transport_manager->addDeviceListener(connection_handler);

  mmh->setProtocolHandler(protocol_handler);
  hmi_handler->setMessageObserver(app_manager);

  protocol_handler->set_session_observer(connection_handler);
  protocol_handler->set_protocol_observer(mmh);

  connection_handler->set_transport_manager(transport_manager);
  connection_handler->set_connection_handler_observer(app_manager);

  app_manager->set_mobile_message_handler(mmh);
  mmh->AddMobileMessageListener(app_manager);
  app_manager->set_connection_handler(connection_handler);
  app_manager->set_hmi_message_handler(hmi_handler);

  // --------------------------------------------------------------------------
  // Third-Party components initialization.

  if (!InitMessageBroker()) {
    exit(EXIT_FAILURE);
  }
  LOG4CXX_INFO(logger, "InitMessageBroker successful");

  if (!InitHmi()) {
    exit(EXIT_FAILURE);
  }
  LOG4CXX_INFO(logger, "InitHmi successful");

  // --------------------------------------------------------------------------
#endif

  while (true) {
    sleep(100500);
  }
}

