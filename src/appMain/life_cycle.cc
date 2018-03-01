/*
* Copyright (c) 2017, Ford Motor Company
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

#include "life_cycle.h"
#include "utils/signals.h"
#include "utils/make_shared.h"
#include "config_profile/profile.h"
#include "resumption/last_state_impl.h"

#ifdef ENABLE_SECURITY
#include "security_manager/security_manager_impl.h"
#include "security_manager/crypto_manager_impl.h"
#include "security_manager/crypto_manager_settings_impl.h"
#include "application_manager/policies/policy_handler.h"
#endif  // ENABLE_SECURITY

#ifdef ENABLE_LOG
#include "utils/log_message_loop_thread.h"
#endif  // ENABLE_LOG

using threads::Thread;

namespace main_namespace {

CREATE_LOGGERPTR_GLOBAL(logger_, "SDLMain")

namespace {
void NameMessageBrokerThread(const System::Thread& thread,
                             const std::string& name) {
  Thread::SetNameForId(thread.GetId(), name);
}

void StopThread(System::Thread* thread) {
  if (thread) {
    thread->Stop();
    thread->Join();
    delete thread;
  }
}
}  // namespace

LifeCycle::LifeCycle(const profile::Profile& profile)
    : transport_manager_(NULL)
    , protocol_handler_(NULL)
    , connection_handler_(NULL)
    , app_manager_(NULL)
#ifdef ENABLE_SECURITY
    , crypto_manager_(NULL)
    , security_manager_(NULL)
#endif  // ENABLE_SECURITY
    , hmi_handler_(NULL)
    , hmi_message_adapter_(NULL)
    , media_manager_(NULL)
    , last_state_(NULL)
#ifdef TELEMETRY_MONITOR
    , telemetry_monitor_(NULL)
#endif  // TELEMETRY_MONITOR
#ifdef DBUS_HMIADAPTER
    , dbus_adapter_(NULL)
    , dbus_adapter_thread_(NULL)
#endif  // DBUS_HMIADAPTER
#ifdef MESSAGEBROKER_HMIADAPTER
    , mb_adapter_(NULL)
    , message_broker_(NULL)
    , message_broker_server_(NULL)
    , mb_thread_(NULL)
    , mb_server_thread_(NULL)
    , mb_adapter_thread_(NULL)
#endif  // MESSAGEBROKER_HMIADAPTER
    , profile_(profile) {
}

bool LifeCycle::StartComponents() {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(!last_state_);
  last_state_ = new resumption::LastStateImpl(profile_.app_storage_folder(),
                                              profile_.app_info_storage());

  DCHECK(!transport_manager_);
  transport_manager_ = new transport_manager::TransportManagerDefault(profile_);

  DCHECK(!connection_handler_);
  connection_handler_ = new connection_handler::ConnectionHandlerImpl(
      profile_, *transport_manager_);

  DCHECK(!protocol_handler_);
  protocol_handler_ =
      new protocol_handler::ProtocolHandlerImpl(profile_,
                                                *connection_handler_,
                                                *connection_handler_,
                                                *transport_manager_);
  DCHECK(protocol_handler_);

  DCHECK(!app_manager_);
  app_manager_ =
      new application_manager::ApplicationManagerImpl(profile_, profile_);

  DCHECK(!hmi_handler_);
  hmi_handler_ = new hmi_message_handler::HMIMessageHandlerImpl(profile_);

  hmi_handler_->set_message_observer(app_manager_);
  app_manager_->set_hmi_message_handler(hmi_handler_);

  media_manager_ = new media_manager::MediaManagerImpl(*app_manager_, profile_);
  app_manager_->set_connection_handler(connection_handler_);
  if (!app_manager_->Init(*last_state_, media_manager_)) {
    LOG4CXX_ERROR(logger_, "Application manager init failed.");
    return false;
  }

#ifdef ENABLE_SECURITY
  security_manager_ = new security_manager::SecurityManagerImpl();
  crypto_manager_ = new security_manager::CryptoManagerImpl(
      utils::MakeShared<security_manager::CryptoManagerSettingsImpl>(
          profile_, app_manager_->GetPolicyHandler().RetrieveCertificate()));
  protocol_handler_->AddProtocolObserver(security_manager_);
  protocol_handler_->set_security_manager(security_manager_);

  security_manager_->set_session_observer(connection_handler_);
  security_manager_->set_protocol_handler(protocol_handler_);
  security_manager_->set_crypto_manager(crypto_manager_);
  security_manager_->AddListener(app_manager_);

  app_manager_->AddPolicyObserver(crypto_manager_);
  if (!crypto_manager_->Init()) {
    LOG4CXX_ERROR(logger_, "CryptoManager initialization fail.");
    return false;
  }
#endif  // ENABLE_SECURITY

  transport_manager_->AddEventListener(protocol_handler_);
  transport_manager_->AddEventListener(connection_handler_);

  protocol_handler_->AddProtocolObserver(media_manager_);
  protocol_handler_->AddProtocolObserver(app_manager_);

  media_manager_->SetProtocolHandler(protocol_handler_);

  connection_handler_->set_protocol_handler(protocol_handler_);
  connection_handler_->set_connection_handler_observer(app_manager_);

// it is important to initialise TelemetryMonitor before TM to listen TM
// Adapters
#ifdef TELEMETRY_MONITOR
  telemetry_monitor_ = new telemetry_monitor::TelemetryMonitor(
      profile_.server_address(), profile_.time_testing_port());
  telemetry_monitor_->Start();
  telemetry_monitor_->Init(protocol_handler_, app_manager_, transport_manager_);
#endif  // TELEMETRY_MONITOR
  // It's important to initialise TM after setting up listener chain
  // [TM -> CH -> AM], otherwise some events from TM could arrive at nowhere
  app_manager_->set_protocol_handler(protocol_handler_);

  if (!transport_manager_->Init(*last_state_)) {
    LOG4CXX_ERROR(logger_, "Transport manager init failed.");
    return false;
  }
  // start transport manager
  transport_manager_->Visibility(true);

  return true;
}

#ifdef MESSAGEBROKER_HMIADAPTER
bool LifeCycle::InitMessageSystem() {
  DCHECK(!message_broker_)
  message_broker_ = NsMessageBroker::CMessageBroker::getInstance();
  if (!message_broker_) {
    LOG4CXX_FATAL(logger_, " Wrong pMessageBroker pointer!");
    return false;
  }

  message_broker_server_ = new NsMessageBroker::TcpServer(
      profile_.server_address(), profile_.server_port(), message_broker_);
  if (!message_broker_server_) {
    LOG4CXX_FATAL(logger_, " Wrong pJSONRPC20Server pointer!");
    return false;
  }
  message_broker_->startMessageBroker(message_broker_server_);
  if (!networking::init()) {
    LOG4CXX_FATAL(logger_, " Networking initialization failed!");
    return false;
  }

  if (!message_broker_server_->Bind()) {
    LOG4CXX_FATAL(logger_, "Message broker server bind failed!");
    return false;
  } else {
    LOG4CXX_INFO(logger_, "Message broker server bind successful!");
  }

  if (!message_broker_server_->Listen()) {
    LOG4CXX_FATAL(logger_, "Message broker server listen failed!");
    return false;
  } else {
    LOG4CXX_INFO(logger_, " Message broker server listen successful!");
  }

  mb_adapter_ = new hmi_message_handler::MessageBrokerAdapter(
      hmi_handler_, profile_.server_address(), profile_.server_port());

  hmi_handler_->AddHMIMessageAdapter(mb_adapter_);
  if (!mb_adapter_->Connect()) {
    LOG4CXX_FATAL(logger_, "Cannot connect to remote peer!");
    return false;
  }

  LOG4CXX_INFO(logger_, "Start CMessageBroker thread!");
  mb_thread_ = new System::Thread(
      new System::ThreadArgImpl<NsMessageBroker::CMessageBroker>(
          *message_broker_,
          &NsMessageBroker::CMessageBroker::MethodForThread,
          NULL));
  mb_thread_->Start(false);
  // Thread can be named only when started because before that point
  // thread doesn't have valid Id to associate name with
  NameMessageBrokerThread(*mb_thread_, "MessageBroker");

  LOG4CXX_INFO(logger_, "Start MessageBroker TCP server thread!");
  mb_server_thread_ =
      new System::Thread(new System::ThreadArgImpl<NsMessageBroker::TcpServer>(
          *message_broker_server_,
          &NsMessageBroker::TcpServer::MethodForThread,
          NULL));
  mb_server_thread_->Start(false);
  NameMessageBrokerThread(*mb_server_thread_, "MB TCPServer");

  LOG4CXX_INFO(logger_, "StartAppMgr JSONRPC 2.0 controller receiver thread!");
  mb_adapter_thread_ = new System::Thread(
      new System::ThreadArgImpl<hmi_message_handler::MessageBrokerAdapter>(
          *mb_adapter_,
          &hmi_message_handler::MessageBrokerAdapter::
              SubscribeAndBeginReceiverThread,
          NULL));
  mb_adapter_thread_->Start(false);
  NameMessageBrokerThread(*mb_adapter_thread_, "MB Adapter");
  return true;
}
#endif  // MESSAGEBROKER_HMIADAPTER

#ifdef DBUS_HMIADAPTER
/**
 * Initialize DBus component
 * @return true if success otherwise false.
 */
bool LifeCycle::InitMessageSystem() {
  dbus_adapter_ = new hmi_message_handler::DBusMessageAdapter(hmi_handler_);

  hmi_handler_.AddHMIMessageAdapter(dbus_adapter_);
  if (!dbus_adapter_->Init()) {
    LOG4CXX_FATAL(logger_, "Cannot init DBus service!");
    return false;
  }

  dbus_adapter_->SubscribeTo();

  LOG4CXX_INFO(logger_, "Start DBusMessageAdapter thread!");
  dbus_adapter_thread_ = new System::Thread(
      new System::ThreadArgImpl<hmi_message_handler::DBusMessageAdapter>(
          *dbus_adapter_,
          &hmi_message_handler::DBusMessageAdapter::MethodForReceiverThread,
          NULL));
  dbus_adapter_thread_->Start(false);

  return true;
}
#endif  // DBUS_HMIADAPTER

namespace {
void sig_handler(int sig) {
  switch (sig) {
    case SIGINT:
      LOG4CXX_DEBUG(logger_, "SIGINT signal has been caught");
      break;
    case SIGTERM:
      LOG4CXX_DEBUG(logger_, "SIGTERM signal has been caught");
      break;
    case SIGSEGV:
      LOG4CXX_DEBUG(logger_, "SIGSEGV signal has been caught");
      FLUSH_LOGGER();
      // exit need to prevent endless sending SIGSEGV
      // http://stackoverflow.com/questions/2663456/how-to-write-a-signal-handler-to-catch-sigsegv
      abort();
    default:
      LOG4CXX_DEBUG(logger_, "Unexpected signal has been caught");
      exit(EXIT_FAILURE);
  }
}
}  //  namespace

void LifeCycle::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  // Register signal handlers and wait sys signals
  // from OS
  if (!utils::WaitTerminationSignals(&sig_handler)) {
    LOG4CXX_FATAL(logger_, "Fail to catch system signal!");
  }
}

void LifeCycle::StopComponents() {
  LOG4CXX_AUTO_TRACE(logger_);

  DCHECK_OR_RETURN_VOID(hmi_handler_);
  hmi_handler_->set_message_observer(NULL);

  DCHECK_OR_RETURN_VOID(connection_handler_);
  connection_handler_->set_connection_handler_observer(NULL);

  DCHECK_OR_RETURN_VOID(protocol_handler_);
  protocol_handler_->RemoveProtocolObserver(app_manager_);

  DCHECK_OR_RETURN_VOID(app_manager_);
  app_manager_->Stop();

  LOG4CXX_INFO(logger_, "Stopping Protocol Handler");
  DCHECK_OR_RETURN_VOID(protocol_handler_);
  protocol_handler_->RemoveProtocolObserver(media_manager_);

#ifdef ENABLE_SECURITY
  protocol_handler_->RemoveProtocolObserver(security_manager_);
  if (security_manager_) {
    security_manager_->RemoveListener(app_manager_);
    LOG4CXX_INFO(logger_, "Destroying Crypto Manager");
    delete crypto_manager_;
    crypto_manager_ = NULL;
    LOG4CXX_INFO(logger_, "Destroying Security Manager");
    delete security_manager_;
    security_manager_ = NULL;
  }
#endif  // ENABLE_SECURITY
  protocol_handler_->Stop();

  LOG4CXX_INFO(logger_, "Destroying Media Manager");
  DCHECK_OR_RETURN_VOID(media_manager_);
  media_manager_->SetProtocolHandler(NULL);
  delete media_manager_;
  media_manager_ = NULL;

  LOG4CXX_INFO(logger_, "Destroying Transport Manager.");
  DCHECK_OR_RETURN_VOID(transport_manager_);
  transport_manager_->Visibility(false);
  transport_manager_->Stop();
  delete transport_manager_;
  transport_manager_ = NULL;

  LOG4CXX_INFO(logger_, "Stopping Connection Handler.");
  DCHECK_OR_RETURN_VOID(connection_handler_);
  connection_handler_->Stop();

  LOG4CXX_INFO(logger_, "Destroying Protocol Handler");
  DCHECK(protocol_handler_);
  delete protocol_handler_;
  protocol_handler_ = NULL;

  LOG4CXX_INFO(logger_, "Destroying Connection Handler.");
  delete connection_handler_;
  connection_handler_ = NULL;

  LOG4CXX_INFO(logger_, "Destroying Last State");
  DCHECK(last_state_);
  delete last_state_;
  last_state_ = NULL;

  LOG4CXX_INFO(logger_, "Destroying Application Manager.");
  DCHECK(app_manager_);
  delete app_manager_;
  app_manager_ = NULL;

  LOG4CXX_INFO(logger_, "Destroying HMI Message Handler and MB adapter.");

#ifdef DBUS_HMIADAPTER
  if (dbus_adapter_) {
    DCHECK_OR_RETURN_VOID(hmi_handler_);
    hmi_handler_->RemoveHMIMessageAdapter(dbus_adapter_);
    dbus_adapter_->exitReceivingThread();
    StopThread(dbus_adapter_thread_);
    delete dbus_adapter_;
    dbus_adapter_ = NULL;
  }
#endif  // DBUS_HMIADAPTER

#ifdef MESSAGEBROKER_HMIADAPTER
  if (mb_adapter_) {
    DCHECK_OR_RETURN_VOID(hmi_handler_);
    hmi_handler_->RemoveHMIMessageAdapter(mb_adapter_);
    mb_adapter_->unregisterController();
    mb_adapter_->exitReceivingThread();
    StopThread(mb_adapter_thread_);
    delete mb_adapter_;
    mb_adapter_ = NULL;
  }

  DCHECK_OR_RETURN_VOID(hmi_handler_);
  delete hmi_handler_;
  hmi_handler_ = NULL;

  LOG4CXX_INFO(logger_, "Destroying Message Broker");
  StopThread(mb_server_thread_);
  StopThread(mb_thread_);

  if (message_broker_server_) {
    message_broker_server_->Close();
    delete message_broker_server_;
    message_broker_server_ = NULL;
  }

  if (message_broker_) {
    message_broker_->stopMessageBroker();
  }

  networking::cleanup();
#endif  // MESSAGEBROKER_HMIADAPTER

#ifdef TELEMETRY_MONITOR
  // It's important to delete tester Obcervers after TM adapters destruction
  if (telemetry_monitor_) {
    telemetry_monitor_->Stop();
    delete telemetry_monitor_;
    telemetry_monitor_ = NULL;
  }
#endif  // TELEMETRY_MONITOR
}

}  //  namespace main_namespace
