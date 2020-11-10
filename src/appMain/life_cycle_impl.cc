/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "appMain/life_cycle_impl.h"
#include "application_manager/system_time/system_time_handler_impl.h"
#include "config_profile/profile.h"
#include "resumption/last_state_impl.h"
#include "resumption/last_state_wrapper_impl.h"
#include "utils/signals.h"
#ifdef ENABLE_SECURITY
#include "application_manager/policies/policy_handler.h"
#include "security_manager/crypto_manager_impl.h"
#include "security_manager/crypto_manager_settings_impl.h"
#include "security_manager/security_manager_impl.h"
#endif  // ENABLE_SECURITY

#include "appMain/low_voltage_signals_handler.h"

using threads::Thread;

namespace main_namespace {

SDL_CREATE_LOG_VARIABLE("SDLMain")

LifeCycleImpl::LifeCycleImpl(const profile::Profile& profile)
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
#ifdef TELEMETRY_MONITOR
    , telemetry_monitor_(NULL)
#endif  // TELEMETRY_MONITOR
#ifdef MESSAGEBROKER_HMIADAPTER
    , mb_adapter_(NULL)
    , mb_adapter_thread_(NULL)
#endif  // MESSAGEBROKER_HMIADAPTER
    , profile_(profile) {
}

bool LifeCycleImpl::StartComponents() {
  SDL_LOG_AUTO_TRACE();
  DCHECK(!last_state_wrapper_);

  auto last_state = std::make_shared<resumption::LastStateImpl>(
      profile_.app_storage_folder(), profile_.app_info_storage());
  last_state_wrapper_ =
      std::make_shared<resumption::LastStateWrapperImpl>(last_state);

  DCHECK(!transport_manager_);
  transport_manager_ = new transport_manager::TransportManagerDefault(
      profile_, transport_manager::TransportAdapterFactory());

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

  auto service_status_update_handler =
      std::unique_ptr<protocol_handler::ServiceStatusUpdateHandler>(
          new protocol_handler::ServiceStatusUpdateHandler(app_manager_));

  protocol_handler_->set_service_status_update_handler(
      std::move(service_status_update_handler));

  DCHECK(!hmi_handler_);
  hmi_handler_ = new hmi_message_handler::HMIMessageHandlerImpl(profile_);

  hmi_handler_->set_message_observer(&(app_manager_->GetRPCHandler()));
  app_manager_->set_hmi_message_handler(hmi_handler_);

  media_manager_ = new media_manager::MediaManagerImpl(*app_manager_, profile_);
  app_manager_->set_connection_handler(connection_handler_);
  app_manager_->AddPolicyObserver(protocol_handler_);
  if (!app_manager_->Init(last_state_wrapper_, media_manager_)) {
    SDL_LOG_ERROR("Application manager init failed.");
    return false;
  }

#ifdef ENABLE_SECURITY
  auto system_time_handler =
      std::unique_ptr<application_manager::SystemTimeHandlerImpl>(
          new application_manager::SystemTimeHandlerImpl(*app_manager_));
  security_manager_ =
      new security_manager::SecurityManagerImpl(std::move(system_time_handler));
  crypto_manager_ = new security_manager::CryptoManagerImpl(
      std::make_shared<security_manager::CryptoManagerSettingsImpl>(
          profile_, app_manager_->GetPolicyHandler().RetrieveCertificate()));
  protocol_handler_->AddProtocolObserver(security_manager_);
  protocol_handler_->set_security_manager(security_manager_);

  security_manager_->set_session_observer(connection_handler_);
  security_manager_->set_protocol_handler(protocol_handler_);
  security_manager_->set_crypto_manager(crypto_manager_);
  security_manager_->AddListener(app_manager_);

  app_manager_->AddPolicyObserver(security_manager_);
  if (!crypto_manager_->Init()) {
    SDL_LOG_ERROR("CryptoManager initialization fail.");
    return false;
  }
#endif  // ENABLE_SECURITY

  transport_manager_->AddEventListener(protocol_handler_);
  transport_manager_->AddEventListener(connection_handler_);

  protocol_handler_->AddProtocolObserver(media_manager_);
  protocol_handler_->AddProtocolObserver(&(app_manager_->GetRPCHandler()));

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
  if (transport_manager::E_SUCCESS !=
      transport_manager_->Init(last_state_wrapper_)) {
    SDL_LOG_ERROR("Transport manager init failed.");
    return false;
  }
  // start transport manager
  transport_manager_->PerformActionOnClients(
      transport_manager::TransportAction::kVisibilityOn);

  LowVoltageSignalsOffset signals_offset{profile_.low_voltage_signal_offset(),
                                         profile_.wake_up_signal_offset(),
                                         profile_.ignition_off_signal_offset()};

  low_voltage_signals_handler_.reset(
      new LowVoltageSignalsHandler(*this, signals_offset));

  return true;
}

void LifeCycleImpl::LowVoltage() {
  SDL_LOG_AUTO_TRACE();
  transport_manager_->PerformActionOnClients(
      transport_manager::TransportAction::kListeningOff);
  transport_manager_->StopEventsProcessing();
  transport_manager_->Deinit();
  app_manager_->OnLowVoltage();
}

void LifeCycleImpl::IgnitionOff() {
  SDL_LOG_AUTO_TRACE();
  kill(getpid(), SIGINT);
}

void LifeCycleImpl::WakeUp() {
  SDL_LOG_AUTO_TRACE();
  transport_manager_->Reinit();
  transport_manager_->PerformActionOnClients(
      transport_manager::TransportAction::kListeningOn);
  app_manager_->OnWakeUp();
  transport_manager_->StartEventsProcessing();
}

#ifdef MESSAGEBROKER_HMIADAPTER
bool LifeCycleImpl::InitMessageSystem() {
  mb_adapter_ = new hmi_message_handler::MessageBrokerAdapter(
      hmi_handler_, profile_.server_address(), profile_.server_port());

  if (!mb_adapter_->StartListener()) {
    return false;
  }

  hmi_handler_->AddHMIMessageAdapter(mb_adapter_);
  mb_adapter_thread_ = new std::thread(
      &hmi_message_handler::MessageBrokerAdapter::Run, mb_adapter_);
  return true;
}
#endif  // MESSAGEBROKER_HMIADAPTER

namespace {
void sig_handler(int sig) {
  switch (sig) {
    case SIGINT:
      SDL_LOG_DEBUG("SIGINT signal has been caught");
      break;
    case SIGTERM:
      SDL_LOG_DEBUG("SIGTERM signal has been caught");
      break;
    case SIGSEGV:
      SDL_LOG_DEBUG("SIGSEGV signal has been caught");
      SDL_FLUSH_LOGGER();
      // exit need to prevent endless sending SIGSEGV
      // http://stackoverflow.com/questions/2663456/how-to-write-a-signal-handler-to-catch-sigsegv
      abort();
    default:
      SDL_LOG_DEBUG("Unexpected signal has been caught");
      exit(EXIT_FAILURE);
  }
}
}  //  namespace

void LifeCycleImpl::Run() {
  SDL_LOG_AUTO_TRACE();
  // Register signal handlers and wait sys signals
  // from OS
  if (!utils::Signals::WaitTerminationSignals(&sig_handler)) {
    SDL_LOG_FATAL("Fail to catch system signal!");
  }
}

void LifeCycleImpl::StopComponents() {
  SDL_LOG_AUTO_TRACE();

  DCHECK_OR_RETURN_VOID(hmi_handler_);
  hmi_handler_->set_message_observer(NULL);

  DCHECK_OR_RETURN_VOID(connection_handler_);
  connection_handler_->set_connection_handler_observer(NULL);

  DCHECK_OR_RETURN_VOID(protocol_handler_);
  protocol_handler_->RemoveProtocolObserver(&(app_manager_->GetRPCHandler()));

  DCHECK_OR_RETURN_VOID(app_manager_);
  app_manager_->Stop();

  SDL_LOG_INFO("Stopping Protocol Handler");
  DCHECK_OR_RETURN_VOID(protocol_handler_);
  protocol_handler_->RemoveProtocolObserver(media_manager_);

#ifdef ENABLE_SECURITY
  protocol_handler_->RemoveProtocolObserver(security_manager_);
  if (security_manager_) {
    security_manager_->RemoveListener(app_manager_);
    SDL_LOG_INFO("Destroying Crypto Manager");
    delete crypto_manager_;
    crypto_manager_ = NULL;
    SDL_LOG_INFO("Destroying Security Manager");
    delete security_manager_;
    security_manager_ = NULL;
  }
#endif  // ENABLE_SECURITY
  protocol_handler_->Stop();

  SDL_LOG_INFO("Destroying Media Manager");
  DCHECK_OR_RETURN_VOID(media_manager_);
  media_manager_->SetProtocolHandler(NULL);
  delete media_manager_;
  media_manager_ = NULL;

  SDL_LOG_INFO("Destroying Transport Manager.");
  DCHECK_OR_RETURN_VOID(transport_manager_);
  transport_manager_->PerformActionOnClients(
      transport_manager::TransportAction::kVisibilityOff);
  transport_manager_->Stop();
  delete transport_manager_;
  transport_manager_ = NULL;

  SDL_LOG_INFO("Stopping Connection Handler.");
  DCHECK_OR_RETURN_VOID(connection_handler_);
  connection_handler_->Stop();

  SDL_LOG_INFO("Destroying Protocol Handler");
  DCHECK(protocol_handler_);
  delete protocol_handler_;
  protocol_handler_ = NULL;

  SDL_LOG_INFO("Destroying Connection Handler.");
  delete connection_handler_;
  connection_handler_ = NULL;

  SDL_LOG_INFO("Destroying Last State.");
  last_state_wrapper_.reset();

  SDL_LOG_INFO("Destroying Application Manager.");
  DCHECK(app_manager_);
  delete app_manager_;
  app_manager_ = NULL;

  SDL_LOG_INFO("Destroying Low Voltage Signals Handler.");
  low_voltage_signals_handler_.reset();

  SDL_LOG_INFO("Destroying HMI Message Handler and MB adapter.");

#ifdef MESSAGEBROKER_HMIADAPTER
  if (mb_adapter_) {
    DCHECK_OR_RETURN_VOID(hmi_handler_);
    hmi_handler_->RemoveHMIMessageAdapter(mb_adapter_);
    mb_adapter_->unregisterController();
    mb_adapter_->exitReceivingThread();
    if (mb_adapter_thread_ != NULL) {
      mb_adapter_thread_->join();
    }
    delete mb_adapter_;
    mb_adapter_ = NULL;
    delete mb_adapter_thread_;
    mb_adapter_thread_ = NULL;
  }
  SDL_LOG_INFO("Destroying Message Broker");
#endif  // MESSAGEBROKER_HMIADAPTER
  DCHECK_OR_RETURN_VOID(hmi_handler_);
  delete hmi_handler_;
  hmi_handler_ = NULL;

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
