/**
* \file signals.cc
* \brief Signal (i.e. SIGINT) handling.
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

#include "./life_cycle.h"
#include "utils/signals.h"
#include "config_profile/profile.h"
#include "security_manager/crypto_manager_impl.h"

using threads::Thread;

namespace main_namespace {
log4cxx::LoggerPtr LifeCycle::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("appMain"));

namespace {

void NameMessageBrokerThread(const System::Thread& thread,
                             const std::string& name) {
  Thread::SetNameForId(Thread::Id(thread.GetId()), name);
}

} // namespace

LifeCycle::LifeCycle()
  : transport_manager_(NULL)
  , protocol_handler_(NULL)
  , connection_handler_(NULL)
  , app_manager_(NULL)
  , crypto_manager_(NULL)
  , security_manager_(NULL)
  , hmi_handler_(NULL)
  , hmi_message_adapter_(NULL)
  , media_manager_(NULL)
  , policy_manager_(NULL)
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
{
}

bool LifeCycle::StartComponents() {
  LOG4CXX_INFO(logger_, "LifeCycle::StartComponents()");
  transport_manager_ =
    transport_manager::TransportManagerDefault::instance();
  DCHECK(transport_manager_ != NULL);

  protocol_handler_ =
    new protocol_handler::ProtocolHandlerImpl(transport_manager_);
  DCHECK(protocol_handler_ != NULL);

  connection_handler_ =
    connection_handler::ConnectionHandlerImpl::instance();
  DCHECK(connection_handler_ != NULL);

  app_manager_ =
    application_manager::ApplicationManagerImpl::instance();
  DCHECK(app_manager_ != NULL);

  hmi_handler_ =
    hmi_message_handler::HMIMessageHandlerImpl::instance();
  DCHECK(hmi_handler_ != NULL)

  security_manager_ = new security_manager::SecurityManager();

  crypto_manager_ = new security_manager::CryptoManagerImpl();
  if(!crypto_manager_->Init()) {
    LOG4CXX_ERROR(logger_, "CryptoManager initialization fail.");
  }

  transport_manager_->AddEventListener(protocol_handler_);
  transport_manager_->AddEventListener(connection_handler_);

  hmi_handler_->set_message_observer(app_manager_);

  media_manager_ = media_manager::MediaManagerImpl::instance();

  protocol_handler_->set_session_observer(connection_handler_);
  protocol_handler_->AddProtocolObserver(media_manager_);
  protocol_handler_->AddProtocolObserver(app_manager_);
  protocol_handler_->AddProtocolObserver(security_manager_);
  media_manager_->SetProtocolHandler(protocol_handler_);

  connection_handler_->set_transport_manager(transport_manager_);
  connection_handler_->set_connection_handler_observer(app_manager_);

  security_manager_->set_session_observer(connection_handler_);
  security_manager_->set_protocol_handler(protocol_handler_);
  security_manager_->set_crypto_manager(crypto_manager_);

  // It's important to initialise TM after setting up listener chain
  // [TM -> CH -> AM], otherwise some events from TM could arrive at nowhere
  transport_manager_->Init();

  policy_manager_ = policies::PolicyManagerImpl::instance();
  DCHECK(policy_manager_);

  policies::PolicyConfiguration policy_config;
  policy_config.set_pt_file_name("wp1_policy_table.json");
  policy_manager_->Init(policy_config);

  app_manager_->set_protocol_handler(protocol_handler_);
  app_manager_->set_connection_handler(connection_handler_);
  app_manager_->set_hmi_message_handler(hmi_handler_);
  app_manager_->set_policy_manager(policy_manager_);

  return true;
}

#ifdef MESSAGEBROKER_HMIADAPTER
bool LifeCycle::InitMessageSystem() {
  message_broker_ =
    NsMessageBroker::CMessageBroker::getInstance();
  if (!message_broker_) {
    LOG4CXX_INFO(logger_, " Wrong pMessageBroker pointer!");
    return false;
  }

  message_broker_server_ =
    new NsMessageBroker::TcpServer(
    profile::Profile::instance()->server_address(),
    profile::Profile::instance()->server_port(),
    message_broker_);
  if (!message_broker_server_) {
    LOG4CXX_INFO(logger_, " Wrong pJSONRPC20Server pointer!");
    return false;
  }
  message_broker_->startMessageBroker(message_broker_server_);
  if (!networking::init()) {
    LOG4CXX_INFO(logger_, " Networking initialization failed!");
    return false;
  }

  if (!message_broker_server_->Bind()) {
    LOG4CXX_FATAL(logger_, "Bind failed!");
    return false;
  } else {
    LOG4CXX_INFO(logger_, "Bind successful!");
  }

  if (!message_broker_server_->Listen()) {
    LOG4CXX_FATAL(logger_, "Listen failed!");
    return false;
  } else {
    LOG4CXX_INFO(logger_, " Listen successful!");
  }

  mb_adapter_ =
    new hmi_message_handler::MessageBrokerAdapter(
    hmi_message_handler::HMIMessageHandlerImpl::instance(),
    profile::Profile::instance()->server_address(),
    profile::Profile::instance()->server_port());

  hmi_message_handler::HMIMessageHandlerImpl::instance()->AddHMIMessageAdapter(
    mb_adapter_);
  if (!mb_adapter_->Connect()) {
    LOG4CXX_INFO(logger_, "Cannot connect to remote peer!");
    return false;
  }

  LOG4CXX_INFO(logger_, "Start CMessageBroker thread!");
  mb_thread_ = new System::Thread(
    new System::ThreadArgImpl<NsMessageBroker::CMessageBroker>(
      *message_broker_, &NsMessageBroker::CMessageBroker::MethodForThread,
      NULL));
  mb_thread_->Start(false);
  // Thread can be named only when started because before that point
  // thread doesn't have valid Id to associate name with
  NameMessageBrokerThread(*mb_thread_, "MessageBrokerThread");

  LOG4CXX_INFO(logger_, "Start MessageBroker TCP server thread!");
  mb_server_thread_  = new System::Thread(
    new System::ThreadArgImpl<NsMessageBroker::TcpServer>(
      *message_broker_server_, &NsMessageBroker::TcpServer::MethodForThread,
      NULL));
  mb_server_thread_->Start(false);
  NameMessageBrokerThread(*mb_server_thread_, "MessageBrokerTCPServerThread");

  LOG4CXX_INFO(logger_, "StartAppMgr JSONRPC 2.0 controller receiver thread!");
  mb_adapter_thread_  = new System::Thread(
    new System::ThreadArgImpl<hmi_message_handler::MessageBrokerAdapter>(
      *mb_adapter_,
      &hmi_message_handler::MessageBrokerAdapter::SubscribeAndBeginReceiverThread,
      NULL));
  mb_adapter_thread_->Start(false);
  NameMessageBrokerThread(*mb_adapter_thread_, "MessageBrokerAdapterThread");

  return true;
}
#endif  // MESSAGEBROKER_HMIADAPTER

#ifdef DBUS_HMIADAPTER
/**
 * Initialize DBus component
 * @return true if success otherwise false.
 */
bool LifeCycle::InitMessageSystem() {
  log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("appMain"));

  dbus_adapter_ = new hmi_message_handler::DBusMessageAdapter(
      hmi_message_handler::HMIMessageHandlerImpl::instance());

  hmi_message_handler::HMIMessageHandlerImpl::instance()->AddHMIMessageAdapter(
      dbus_adapter_);
  if (!dbus_adapter_->Init()) {
    LOG4CXX_INFO(logger, "Cannot init DBus service!");
    return false;
  }

  dbus_adapter_->SubscribeTo();

  LOG4CXX_INFO(logger, "Start DBusMessageAdapter thread!");
  dbus_adapter_thread_ = new System::Thread(
      new System::ThreadArgImpl<hmi_message_handler::DBusMessageAdapter>(
          *dbus_adapter_,
          &hmi_message_handler::DBusMessageAdapter::MethodForReceiverThread,
          NULL));
  dbus_adapter_thread_->Start(false);

  return true;
}
#endif  // DBUS_HMIADAPTER

#ifdef MQUEUE_HMIADAPTER
bool LifeCycle::InitMessageSystem() {
  hmi_message_adapter_ = new hmi_message_handler::MqueueAdapter(
      hmi_message_handler::HMIMessageHandlerImpl::instance());
  hmi_message_handler::HMIMessageHandlerImpl::instance()->AddHMIMessageAdapter(
      hmi_message_adapter_);
  return true;
}
#endif  // MQUEUE_HMIADAPTER

void LifeCycle::StopComponents() {
  hmi_handler_->set_message_observer(NULL);
  connection_handler_->set_connection_handler_observer(NULL);
  protocol_handler_->RemoveProtocolObserver(app_manager_);

  LOG4CXX_INFO(logger_, "Destroying Application Manager.");
  app_manager_->Stop();
  application_manager::ApplicationManagerImpl::destroy();

  LOG4CXX_INFO(logger_, "Destroying Policy Manager.");
  policies::PolicyManagerImpl::destroy();

  LOG4CXX_INFO(logger_, "Destroying Transport Manager.");
  transport_manager_->Stop();
  transport_manager::TransportManagerDefault::destroy();

  LOG4CXX_INFO(logger_, "Destroying Media Manager");
  protocol_handler_->RemoveProtocolObserver(media_manager_);
  media_manager_->SetProtocolHandler(NULL);
  media_manager::MediaManagerImpl::destroy();

  LOG4CXX_INFO(logger_, "Destroying Connection Handler.");
  //TODO: (EZamakhov) set_session_observer(NULL) do nothing
  protocol_handler_->set_session_observer(NULL);
  connection_handler::ConnectionHandlerImpl::destroy();

  LOG4CXX_INFO(logger_, "Destroying Protocol Handler");
  delete protocol_handler_;

  LOG4CXX_INFO(logger_, "Destroying Protocol Handler");
  delete crypto_manager_;

  LOG4CXX_INFO(logger_, "Destroying Security Manager");
  delete security_manager_;

  crypto_manager_->Finish();
  delete crypto_manager_;

  LOG4CXX_INFO(logger_, "Destroying HMI Message Handler and MB adapter.");
#ifdef DBUS_HMIADAPTER
  if (dbus_adapter_) {
    if (hmi_handler_) {
      hmi_handler_->RemoveHMIMessageAdapter(dbus_adapter_);
      hmi_message_handler::HMIMessageHandlerImpl::destroy();
    }
    if (dbus_adapter_thread_) {
      dbus_adapter_thread_->Stop();
      dbus_adapter_thread_->Join();
      delete dbus_adapter_thread_;
    }
    delete dbus_adapter_;
  }
#endif  // DBUS_HMIADAPTER
#ifdef MESSAGEBROKER_HMIADAPTER
  hmi_handler_->RemoveHMIMessageAdapter(mb_adapter_);
  hmi_message_handler::HMIMessageHandlerImpl::destroy();
  mb_adapter_->unregisterController();
  mb_adapter_thread_->Stop();
  mb_adapter_thread_->Join();
  delete mb_adapter_thread_;
  mb_adapter_->Close();
  delete mb_adapter_;
#endif  // MESSAGEBROKER_HMIADAPTER

#ifdef MESSAGEBROKER_HMIADAPTER
  LOG4CXX_INFO(logger_, "Destroying Message Broker");
  mb_server_thread_->Stop();
  mb_server_thread_->Join();
  mb_thread_->Stop();
  mb_thread_->Join();
  message_broker_server_->Close();
  message_broker_->stopMessageBroker();
  delete mb_server_thread_;

  networking::cleanup();
#endif  // MESSAGEBROKER_HMIADAPTER

  delete hmi_message_adapter_;
  hmi_message_adapter_ = NULL;
}

void LifeCycle::StopComponentsOnSignal(int32_t params) {
  utils::ResetSubscribeToTerminateSignal();
  instance()->StopComponents();
  utils::ForwardSignal();
}

}  //  namespace main_namespace
