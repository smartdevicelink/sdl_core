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
 *  LifeCycle: Instantiates, runs, and cleans up the components of the system.
 *
 */

#ifndef APPMAIN_LIFE_CYCLE_H_
#define APPMAIN_LIFE_CYCLE_H_

#include "application_manager/application_manager_impl.h"
#include "connection_handler/connection_handler_impl.h"
#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "media_manager/media_manager_impl.h"
#include "protocol_handler/protocol_handler_impl.h"
// TODO(MW): Update when cmake include paths are fixed,
//           resolves to src/3rd_party-static/MessageBroker/include/system.h
#include "system.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_default.h"
#include "utils/singleton.h"
#include "utils/macro.h"
#ifdef DBUS_HMIADAPTER
# include "hmi_message_handler/dbus_message_adapter.h"
#endif  // DBUS_HMIADAPTER
#if (defined(MESSAGEBROKER_HMIADAPTER) || defined(PASA_HMI))
# include "hmi_message_handler/messagebroker_adapter.h"
#endif  // #if (defined(MESSAGEBROKER_HMIADAPTER) || defined(PASA_HMI))
#ifdef MESSAGEBROKER_HMIADAPTER
// TODO(MW): Update when cmake include paths are fixed,
//           resolves to src/3rd_party-static/MessageBroker/include/CMessageBroker.hpp
# include "CMessageBroker.hpp"
// TODO(MW): Update when cmake include paths are fixed,
//           resolves to src/3rd_party-static/MessageBroker/include/mb_tcpserver.h
# include "mb_tcpserver.hpp"
// TODO(MW): Update when cmake include paths are fixed,
//           resolves to src/3rd_party-static/MessageBroker/include/networking.h
# include "networking.h"
#endif  // MESSAGEBROKER_HMIADAPTER
#ifdef MQUEUE_HMIADAPTER
# include "hmi_message_handler/mqueue_adapter.h"
#endif  // MQUEUE_HMIADAPTER
#ifdef TIME_TESTER
# include "time_tester/time_manager.h"
#endif


#ifdef ENABLE_SECURITY
namespace security_manager {
class CryptoManager;
class SecurityManagerImpl;
}  // namespace security_manager
#endif  // ENABLE_SECURITY

namespace main_namespace {
// Utility class contains much of the initalization and main-loop functionality
// of the program. All methods should be accessed through the instance() method
// of the parent Singleton class. Sample usage:
//    LifeCycle::instance()->StartComponents();
//    LifeCycle::instance()->Run();
//    LifeCycle::instance()->StopComponents();
class LifeCycle : public utils::Singleton<LifeCycle> {
 public:
  // Initializes MessageBroker component. Returns true if successful.
  bool InitMessageSystem();

  // Runs the main loop of the program.
  void Run();

  // Instantiates and initializes handlers/managers and stores their pointers.
  // Sets components_started_ to true to mark object as valid.
  // Returns true if successful.
  bool StartComponents();

  // Stops all running components and cleans up all member pointers.
  //
  // This method can be called either as a final cleanup or to fail gracefully
  // after an unsuccessful initialization.
  // TODO(MW): This functionality should be moved into the desctructor to be
  //           RAII-compliant. Exit statements in main() will have to be
  //           converted to returns.
  void StopComponents();

 private:
  // Instantiates the object, but leaves initialization of the members to the
  // StartComponents() method. Created object is in an invalid state until
  // StartComponents() is called.
  //
  // Constructors of classes that inherit from Singleton must be private.
  LifeCycle();

  application_manager::ApplicationManagerImpl *app_manager_;
  bool components_started_;
  connection_handler::ConnectionHandlerImpl *connection_handler_;
  hmi_message_handler::HMIMessageHandlerImpl *hmi_handler_;
  hmi_message_handler::HMIMessageAdapter *hmi_message_adapter_;
  media_manager::MediaManagerImpl *media_manager_;
  protocol_handler::ProtocolHandlerImpl *protocol_handler_;
  transport_manager::TransportManager *transport_manager_;
#ifdef DBUS_HMIADAPTER
  hmi_message_handler::DBusMessageAdapter *dbus_adapter_;
  System::Thread *dbus_adapter_thread_;
#endif  // DBUS_HMIADAPTER
#ifdef ENABLE_SECURITY
  security_manager::CryptoManager *crypto_manager_;
  security_manager::SecurityManager *security_manager_;
#endif  // ENABLE_SECURITY
#ifdef MESSAGEBROKER_HMIADAPTER
  NsMessageBroker::CMessageBroker *message_broker_;
  hmi_message_handler::MessageBrokerAdapter *message_broker_adapter_;
  System::Thread *message_broker_adapter_thread_;
  NsMessageBroker::TcpServer *message_broker_server_;
  System::Thread *message_broker_server_thread_;
  System::Thread *message_broker_thread_;
#endif  // MESSAGEBROKER_HMIADAPTER
#ifdef TIME_TESTER
  time_tester::TimeManager *time_tester_;
#endif  // TIME_TESTER

  FRIEND_BASE_SINGLETON_CLASS(LifeCycle);
  DISALLOW_COPY_AND_ASSIGN(LifeCycle);
};
}  //  namespace main_namespace

#endif  // APPMAIN_LIFE_CYCLE_H_
