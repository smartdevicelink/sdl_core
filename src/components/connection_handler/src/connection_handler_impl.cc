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
 */
#include <strings.h>

#include <algorithm>
#include <list>
#include <memory>
#include <string>

#include "connection_handler/connection_handler_impl.h"
#include "encryption/hashing.h"
#include "transport_manager/info.h"

#ifdef ENABLE_SECURITY
#include "security_manager/security_manager.h"
#endif  // ENABLE_SECURITY

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

SDL_CREATE_LOG_VARIABLE("ConnectionHandler")

ConnectionHandle HandleFromConnectionUID(transport_manager::ConnectionUID uid) {
  return ConnectionHandle(uid);
}

transport_manager::ConnectionUID ConnectionUIDFromHandle(
    ConnectionHandle handle) {
  return transport_manager::ConnectionUID(handle);
}

ConnectionHandlerImpl::ConnectionHandlerImpl(
    const ConnectionHandlerSettings& settings,
    transport_manager::TransportManager& tm)
    : settings_(settings)
    , connection_handler_observer_(NULL)
    , transport_manager_(tm)
    , protocol_handler_(NULL)
    , session_connection_map_lock_ptr_(
          std::make_shared<sync_primitives::RecursiveLock>())
    , connection_list_lock_()
    , connection_handler_observer_lock_()
    , connection_list_deleter_(&connection_list_)
    , start_service_context_map_lock_()
    , start_service_context_map_()
    , ending_connection_(NULL) {}

ConnectionHandlerImpl::~ConnectionHandlerImpl() {
  SDL_LOG_AUTO_TRACE();
}

void ConnectionHandlerImpl::Stop() {
  SDL_LOG_AUTO_TRACE();
  ConnectionList::iterator itr = connection_list_.begin();
  while (itr != connection_list_.end()) {
    RemoveConnection(itr->second->connection_handle());
    itr = connection_list_.begin();
  }

  sync_primitives::AutoLock auto_lock(start_service_context_map_lock_);
  start_service_context_map_.clear();
}

void ConnectionHandlerImpl::set_connection_handler_observer(
    ConnectionHandlerObserver* observer) {
  SDL_LOG_DEBUG("ConnectionHandlerImpl::set_connection_handler_observer() "
                << observer);
  sync_primitives::AutoWriteLock write_lock(connection_handler_observer_lock_);
  if (!observer) {
    SDL_LOG_WARN("Set Null pointer to observer.");
  }
  connection_handler_observer_ = observer;
}

void ConnectionHandlerImpl::set_protocol_handler(
    protocol_handler::ProtocolHandler* protocol_handler) {
  SDL_LOG_DEBUG("ConnectionHandlerImpl::set_protocol_handler() "
                << protocol_handler);
  if (!protocol_handler) {
    SDL_LOG_WARN("Set Null pointer to protocol handler.");
  }
  protocol_handler_ = protocol_handler;
}

void ConnectionHandlerImpl::OnDeviceListUpdated(
    const std::vector<transport_manager::DeviceInfo>&) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  sync_primitives::AutoReadLock lock(device_list_lock_);
  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

void ConnectionHandlerImpl::OnFindNewApplicationsRequest() {
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_) {
    connection_handler_observer_->OnFindNewApplicationsRequest();
  }
}

void ConnectionHandlerImpl::OnDeviceFound(
    const transport_manager::DeviceInfo&) {
  SDL_LOG_AUTO_TRACE();
}

void ConnectionHandlerImpl::OnDeviceAdded(
    const transport_manager::DeviceInfo& device_info) {
  SDL_LOG_AUTO_TRACE();
  auto handle = device_info.device_handle();

  SDL_LOG_DEBUG("OnDeviceAdded!!!: " << handle << " " << device_info.name()
                                     << " " << device_info.mac_address() << " "
                                     << device_info.connection_type());

  Device device(handle,
                device_info.name(),
                device_info.mac_address(),
                device_info.connection_type());

  {
    sync_primitives::AutoWriteLock write_lock(device_list_lock_);
    auto result = device_list_.insert(std::make_pair(handle, device));

    if (!result.second) {
      SDL_LOG_ERROR("Device with handle " << handle
                                          << " is known already. "
                                             "Information won't be updated.");
      return;
    }
  }
  if (device_info.name() ==
      transport_manager::webengine_constants::kWebEngineDeviceName) {
    connection_handler_observer_->OnWebEngineDeviceCreated();
  }
}

void ConnectionHandlerImpl::OnDeviceRemoved(
    const transport_manager::DeviceInfo& device_info) {
  SDL_LOG_AUTO_TRACE();
  // Device has been removed. Perform all needed actions.
  // 1. Delete all the connections and sessions of this device
  // 2. Delete device from a list

  std::vector<ConnectionHandle> connections_to_remove;
  {
    sync_primitives::AutoReadLock lock(connection_list_lock_);
    for (ConnectionList::iterator it = connection_list_.begin();
         it != connection_list_.end();
         ++it) {
      if (device_info.device_handle() ==
          (*it).second->connection_device_handle()) {
        connections_to_remove.push_back((*it).first);
      }
    }
  }

  std::vector<ConnectionHandle>::iterator it = connections_to_remove.begin();
  for (; it != connections_to_remove.end(); ++it) {
    RemoveConnection(*it);
  }

  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  {
    sync_primitives::AutoWriteLock lock(device_list_lock_);
    device_list_.erase(device_info.device_handle());
  }
  if (connection_handler_observer_) {
    connection_handler_observer_->RemoveDevice(device_info.device_handle());
  }
}

void ConnectionHandlerImpl::OnDeviceSwitchingFinish(
    const transport_manager::DeviceUID& device_uid) {
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceSwitchingFinish(
        encryption::MakeHash(device_uid));
  }
}

namespace {
struct DeviceFinder {
  explicit DeviceFinder(const std::string& device_uid)
      : device_uid_(device_uid) {}
  bool operator()(const DeviceMap::value_type& device) const {
    return device_uid_ == device.second.mac_address();
  }

 private:
  const std::string& device_uid_;
};
}  // namespace

void ConnectionHandlerImpl::OnDeviceSwitchingStart(
    const std::string& device_uid_from, const std::string& device_uid_to) {
  sync_primitives::AutoReadLock lock(device_list_lock_);

  auto device_from =
      std::find_if(device_list_.begin(),
                   device_list_.end(),
                   DeviceFinder(encryption::MakeHash(device_uid_from)));

  auto device_to =
      std::find_if(device_list_.begin(),
                   device_list_.end(),
                   DeviceFinder(encryption::MakeHash(device_uid_to)));

  DCHECK_OR_RETURN_VOID(device_list_.end() != device_from);
  DCHECK_OR_RETURN_VOID(device_list_.end() != device_to);
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceSwitchingStart(device_from->second,
                                                         device_to->second);
  }
}

void ConnectionHandlerImpl::OnScanDevicesFinished() {
  SDL_LOG_AUTO_TRACE();
}

void ConnectionHandlerImpl::OnScanDevicesFailed(
    const transport_manager::SearchDeviceError& error) {
  SDL_LOG_WARN("Scan devices failed. " << error.text());
}

void ConnectionHandlerImpl::OnConnectionStatusUpdated() {
  connection_handler_observer_->OnConnectionStatusUpdated();
}

void ConnectionHandlerImpl::OnConnectionPending(
    const transport_manager::DeviceInfo& device_info,
    const transport_manager::ConnectionUID connection_id) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("OnConnectionPending!!!: " << device_info.device_handle() << " "
                                           << device_info.name() << " "
                                           << device_info.mac_address() << " "
                                           << device_info.connection_type());
  {
    sync_primitives::AutoReadLock lock(device_list_lock_);
    auto it = device_list_.find(device_info.device_handle());

    if (device_list_.end() == it) {
      SDL_LOG_ERROR("Unknown device!");
      return;
    }
  }
  SDL_LOG_DEBUG("Add Pending Connection #" << connection_id << " to the list.");

  std::string endpoint = device_info.mac_address();
  cloud_app_id_map_lock_.Acquire();
  for (auto it = cloud_app_id_map_.begin(); it != cloud_app_id_map_.end();
       ++it) {
    if (endpoint == it->second.first) {
      it->second.second = connection_id;
      break;
    }
  }
  cloud_app_id_map_lock_.Release();

  sync_primitives::AutoWriteLock lock(connection_list_lock_);
  if (connection_list_.find(connection_id) == connection_list_.end()) {
    Connection* connection =
        new Connection(connection_id,
                       device_info.device_handle(),
                       this,
                       get_settings().heart_beat_timeout());

    connection_list_.insert(
        ConnectionList::value_type(connection_id, connection));

    connection_handler::DeviceHandle device_id =
        connection->connection_device_handle();

    connection_handler_observer_->CreatePendingApplication(
        connection_id, device_info, device_id);
  } else {
    connection_handler_observer_->SetPendingApplicationState(connection_id,
                                                             device_info);
  }
}

void ConnectionHandlerImpl::OnConnectionEstablished(
    const transport_manager::DeviceInfo& device_info,
    const transport_manager::ConnectionUID connection_id) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("OnConnectionEstablished!!!: "
                << device_info.device_handle() << " " << device_info.name()
                << " " << device_info.mac_address() << " "
                << device_info.connection_type());
  {
    sync_primitives::AutoReadLock lock(device_list_lock_);
    auto it = device_list_.find(device_info.device_handle());
    if (device_list_.end() == it) {
      SDL_LOG_ERROR("Unknown device!");
      return;
    }
  }

  SDL_LOG_DEBUG("Add Connection #" << connection_id << " to the list.");
  sync_primitives::AutoWriteLock lock(connection_list_lock_);
  if (connection_list_.find(connection_id) == connection_list_.end()) {
    connection_list_.insert(ConnectionList::value_type(
        connection_id,
        new Connection(connection_id,
                       device_info.device_handle(),
                       this,
                       get_settings().heart_beat_timeout())));
  }
}

void ConnectionHandlerImpl::OnConnectionFailed(
    const transport_manager::DeviceInfo& device_info,
    const transport_manager::ConnectError& error) {
  // TODO(PV): implement
  SDL_LOG_ERROR("Failed connecting.");
}

void ConnectionHandlerImpl::OnConnectionClosed(
    transport_manager::ConnectionUID connection_id) {
  SDL_LOG_AUTO_TRACE();

  OnConnectionEnded(connection_id);
}

void ConnectionHandlerImpl::OnConnectionClosedFailure(
    transport_manager::ConnectionUID connection_id,
    const transport_manager::DisconnectError& error) {
  // TODO(PV): implement
  SDL_LOG_ERROR("ConnectionHandlerImpl::OnConnectionClosedFailure");
}

void ConnectionHandlerImpl::OnUnexpectedDisconnect(
    transport_manager::ConnectionUID connection_id,
    const transport_manager::CommunicationError& error) {
  SDL_LOG_AUTO_TRACE();
  UNUSED(error);
  OnConnectionEnded(connection_id);
}

void ConnectionHandlerImpl::OnDeviceConnectionLost(
    const transport_manager::DeviceHandle& device,
    const transport_manager::DisconnectDeviceError& error) {
  // TODO(PV): implement
  SDL_LOG_ERROR("Lost connection with device " << device);
}

void ConnectionHandlerImpl::OnDisconnectFailed(
    const transport_manager::DeviceHandle& device,
    const transport_manager::DisconnectDeviceError& error) {
  // TODO(PV): implement
  SDL_LOG_ERROR("Trying to Disconnect device failed.");
}

void ConnectionHandlerImpl::RemoveConnection(
    const ConnectionHandle connection_handle) {
  SDL_LOG_AUTO_TRACE();

  OnConnectionEnded(connection_handle);
}

#ifdef ENABLE_SECURITY
bool AllowProtection(const ConnectionHandlerSettings& settings,
                     const protocol_handler::ServiceType& service_type,
                     const bool is_protected) {
  SDL_LOG_AUTO_TRACE();
  const std::vector<int>& force_unprotected_list =
      is_protected ? settings.force_unprotected_service()
                   : settings.force_protected_service();

  if (std::find(force_unprotected_list.begin(),
                force_unprotected_list.end(),
                service_type) != force_unprotected_list.end()) {
    SDL_LOG_ERROR("Service " << static_cast<int>(service_type)
                             << " shall be protected");
    return false;
  }
  SDL_LOG_DEBUG("Service " << static_cast<int>(service_type) << " allowed");
  return true;
}
#endif  // ENABLE_SECURITY

void ConnectionHandlerImpl::OnSessionStartedCallback(
    const transport_manager::ConnectionUID connection_handle,
    const uint8_t session_id,
    const protocol_handler::ServiceType& service_type,
    const bool is_protected,
    const BsonObject* params) {
  SDL_LOG_AUTO_TRACE();

  std::vector<std::string> rejected_params;

  // In case this is a Session running on a Secondary Transport, we need to
  // find the Sessions's primary transport. In this case, "connection_handle"
  // reflects the secondary transport, which we need for the various
  // callbacks, so they can send appropriate Ack or NAK messages on the
  // correct transport.
  transport_manager::ConnectionUID primary_connection_handle =
      connection_handle;
  SessionTransports st = GetSessionTransports(session_id);
  if (session_id != 0) {
    if (st.primary_transport == 0) {
      SDL_LOG_WARN(
          "OnSessionStartedCallback could not find Session in the "
          "Session/Connection Map!");
    } else {
      SDL_LOG_INFO("OnSessionStartedCallback found session "
                   << static_cast<int>(session_id)
                   << " with primary connection "
                   << static_cast<int>(st.primary_transport)
                   << " and secondary connection "
                   << static_cast<int>(st.secondary_transport));
      primary_connection_handle = st.primary_transport;
    }
  }

  protocol_handler::SessionContext context(primary_connection_handle,
                                           connection_handle,
                                           session_id,
                                           0,
                                           service_type,
                                           protocol_handler::HASH_ID_WRONG,
                                           is_protected);

#ifdef ENABLE_SECURITY
  if (!AllowProtection(get_settings(), service_type, is_protected)) {
    protocol_handler_->NotifySessionStarted(
        context,
        rejected_params,
        "Service of type " + std::to_string(service_type) + " cannot be " +
            (is_protected ? "protected" : "unprotected"));
    return;
  }
#endif  // ENABLE_SECURITY

  auto find_connection =
      [this](const transport_manager::ConnectionUID& primary_connection_handle)
      -> Connection* {
    sync_primitives::AutoReadLock lock(connection_list_lock_);
    auto it = connection_list_.find(primary_connection_handle);
    if (it != connection_list_.end()) {
      return it->second;
    }
    return nullptr;
  };

  Connection* connection = find_connection(primary_connection_handle);

  if (!connection) {
    SDL_LOG_ERROR("Unknown connection!");
    protocol_handler_->NotifySessionStarted(
        context,
        rejected_params,
        "Unknown connection " + std::to_string(primary_connection_handle));
    return;
  }

  context.is_new_service_ =
      !connection->SessionServiceExists(session_id, service_type);

  if ((0 == session_id) && (protocol_handler::kRpc == service_type)) {
    context.new_session_id_ =
        connection->AddNewSession(primary_connection_handle);
    if (0 == context.new_session_id_) {
      SDL_LOG_ERROR("Couldn't start new session!");
      protocol_handler_->NotifySessionStarted(
          context, rejected_params, "Unable to create new session");
      return;
    }
    context.hash_id_ =
        KeyFromPair(primary_connection_handle, context.new_session_id_);
  } else {  // Could be create new service or protected exists one
    std::string err_reason;
    if (!connection->AddNewService(session_id,
                                   service_type,
                                   is_protected,
                                   connection_handle,
                                   &err_reason)) {
      SDL_LOG_ERROR("Couldn't establish "
#ifdef ENABLE_SECURITY
                    << (is_protected ? "protected" : "non-protected")
#endif  // ENABLE_SECURITY
                    << " service " << static_cast<int>(service_type)
                    << " for session " << static_cast<int>(session_id));

      protocol_handler_->NotifySessionStarted(
          context,
          rejected_params,
          "Cannot start " +
              std::string(is_protected ? "a protected" : "an unprotected") +
              " service of type " + std::to_string(service_type) + ". " +
              err_reason);
      return;
    }
    context.new_session_id_ = session_id;
    context.hash_id_ = protocol_handler::HASH_ID_NOT_SUPPORTED;

    if (st.secondary_transport == connection_handle) {
      sync_primitives::AutoLock auto_lock(session_connection_map_lock_ptr_);
      st.secondary_transport_services.push_back(service_type);
    }
  }
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_) {
    const uint32_t session_key =
        KeyFromPair(primary_connection_handle, context.new_session_id_);

    {
      sync_primitives::AutoLock auto_lock(start_service_context_map_lock_);
      start_service_context_map_[session_key] = context;
    }

    connection_handler_observer_->OnServiceStartedCallback(
        connection->connection_device_handle(),
        session_key,
        service_type,
        params);
  } else {
    if (protocol_handler_) {
      protocol_handler_->NotifySessionStarted(context, rejected_params);
    }
  }
}

void ConnectionHandlerImpl::NotifyServiceStartedResult(
    uint32_t session_key,
    bool result,
    std::vector<std::string>& rejected_params,
    const std::string& reason) {
  SDL_LOG_AUTO_TRACE();

  protocol_handler::SessionContext context;
  {
    sync_primitives::AutoLock auto_lock(start_service_context_map_lock_);
    auto it = start_service_context_map_.find(session_key);
    if (it == start_service_context_map_.end()) {
      SDL_LOG_ERROR("context for start service not found!");
      return;
    }
    context = it->second;
    start_service_context_map_.erase(it);
  }

  // We need the context's primary connection so we can manage its services
  // list
  Connection* connection = NULL;
  {
    sync_primitives::AutoReadLock lock(connection_list_lock_);
    ConnectionList::iterator it =
        connection_list_.find(context.primary_connection_id_);
    if (connection_list_.end() == it) {
      SDL_LOG_ERROR("connection not found");
      return;
    }
    connection = it->second;
  }

  if (!result) {
    SDL_LOG_WARN("Service starting forbidden by connection_handler_observer");
    if (protocol_handler::kRpc == context.service_type_) {
      connection->RemoveSession(context.new_session_id_);
    } else {
      connection->RemoveService(context.initial_session_id_,
                                context.service_type_);
    }
    context.new_session_id_ = 0;
  }

  if (protocol_handler_ != NULL) {
    protocol_handler_->NotifySessionStarted(context, rejected_params, reason);
  }
}

void ConnectionHandlerImpl::OnApplicationFloodCallBack(
    const uint32_t& connection_key) {
  SDL_LOG_AUTO_TRACE();

  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  // In case this is a Session running on a Secondary Transport,
  // "connection_handle" will reflect the active (secondary) transport.
  // To close the conneciton and its sessions properly, we need to find
  // the Sessions's primary transport/connection.
  if (session_id != 0) {
    SessionTransports st = GetSessionTransports(session_id);
    if (st.primary_transport == 0) {
      SDL_LOG_WARN(
          "OnApplicationFloodCallBack could not find Session in the "
          "Session/Connection Map!");
    } else {
      SDL_LOG_INFO("OnApplicationFloodCallBack found session "
                   << static_cast<int>(session_id)
                   << " with primary connection "
                   << static_cast<int>(st.primary_transport)
                   << " and secondary connection "
                   << static_cast<int>(st.secondary_transport));
      connection_handle = st.primary_transport;
    }
  }

  SDL_LOG_INFO("Disconnect flooding application");
  if (session_id != 0) {
    CloseSession(connection_handle, session_id, kFlood);
  } else {
    CloseConnectionSessions(connection_handle, kCommon);
    CloseConnection(connection_handle);
  }
}

void ConnectionHandlerImpl::OnMalformedMessageCallback(
    const uint32_t& connection_key) {
  SDL_LOG_AUTO_TRACE();

  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  // In case this is a Session running on a Secondary Transport,
  // "connection_handle" will reflect the active (secondary) transport.
  // To close the conneciton and its sessions properly, we need to find
  // the Sessions's primary transport/connection.
  if (session_id != 0) {
    SessionTransports st = GetSessionTransports(session_id);
    if (st.primary_transport == 0) {
      SDL_LOG_WARN(
          "OnMalformedMessageCallback could not find Session in the "
          "Session/Connection Map!");
    } else {
      SDL_LOG_INFO("OnMalformedMessageCallback found session "
                   << static_cast<int>(session_id)
                   << " with primary connection "
                   << static_cast<int>(st.primary_transport)
                   << " and secondary connection "
                   << static_cast<int>(st.secondary_transport));
      connection_handle = st.primary_transport;
    }
  }

  SDL_LOG_INFO("Disconnect malformed messaging application");
  CloseConnectionSessions(connection_handle, kMalformed);
  CloseConnection(connection_handle);
}

void ConnectionHandlerImpl::OnFinalMessageCallback(
    const uint32_t& connection_key) {
  SDL_LOG_AUTO_TRACE();

  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoWriteLock connection_list_lock(connection_list_lock_);
  ConnectionList::iterator connection_it =
      connection_list_.find(connection_handle);

  if (connection_list_.end() != connection_it) {
    SDL_LOG_DEBUG("OnFinalMessageCallback found connection "
                  << connection_handle);
    connection_it->second->OnFinalMessageCallback();
  }
}

uint32_t ConnectionHandlerImpl::OnSessionEndedCallback(
    const transport_manager::ConnectionUID connection_handle,
    const uint8_t session_id,
    uint32_t* hashCode,
    const protocol_handler::ServiceType& service_type,
    std::string* err_reason) {
  SDL_LOG_AUTO_TRACE();

  // In case this is a Session running on a Secondary Transport, we need to
  // find the Sessions's primary transport. In this case, "connection_handle"
  // reflects the secondary transport, which we need for the various
  // callbacks, so they can send appropriate Ack or NAK messages on the
  // correct transport.
  transport_manager::ConnectionUID primary_connection_handle =
      connection_handle;
  if (session_id != 0) {
    SessionTransports st = GetSessionTransports(session_id);
    if (st.primary_transport == 0) {
      SDL_LOG_WARN(
          "OnSessionEndedCallback could not find Session in the "
          "Session/Connection Map!");
      if (err_reason) {
        *err_reason = "Could not find Session in the Session/Connection Map!";
      }
    } else {
      SDL_LOG_INFO("OnSessionEndedCallback found session "
                   << static_cast<int>(session_id)
                   << " with primary connection "
                   << static_cast<int>(st.primary_transport)
                   << " and secondary connection "
                   << static_cast<int>(st.secondary_transport));
      primary_connection_handle = st.primary_transport;
    }
  }

  connection_list_lock_.AcquireForReading();
  ConnectionList::iterator it =
      connection_list_.find(primary_connection_handle);
  if (connection_list_.end() == it) {
    SDL_LOG_WARN("Unknown connection!");
    connection_list_lock_.Release();
    if (err_reason) {
      *err_reason = "Could not find Connection in the Connection Map!";
    }
    return 0;
  }
  std::pair<int32_t, Connection*> connection_item = *it;
  connection_list_lock_.Release();

  Connection* connection = connection_item.second;
  const uint32_t session_key =
      KeyFromPair(primary_connection_handle, session_id);

  if (protocol_handler::kRpc == service_type) {
    SDL_LOG_INFO("Session " << static_cast<uint32_t>(session_id)
                            << " to be removed");
    // old version of protocol doesn't support hash
    if (protocol_handler::HASH_ID_NOT_SUPPORTED != *hashCode) {
      if (protocol_handler::HASH_ID_WRONG == *hashCode ||
          session_key != *hashCode) {
        SDL_LOG_WARN("Wrong hash_id for session "
                     << static_cast<uint32_t>(session_id));
        *hashCode = protocol_handler::HASH_ID_WRONG;

        if (err_reason) {
          *err_reason = "Wrong hash_id for session " +
                        std::to_string(static_cast<uint32_t>(session_id));
        }
        return 0;
      }
    }
    if (!connection->RemoveSession(session_id)) {
      SDL_LOG_WARN("Couldn't remove session "
                   << static_cast<uint32_t>(session_id));
      if (err_reason) {
        *err_reason = "Couldn't remove session " +
                      std::to_string(static_cast<uint32_t>(session_id));
      }
      return 0;
    }
  } else {
    SDL_LOG_INFO("Service " << static_cast<uint32_t>(service_type)
                            << " to be removed");
    if (!connection->RemoveService(session_id, service_type)) {
      SDL_LOG_WARN("Couldn't remove service "
                   << static_cast<uint32_t>(service_type));
      if (err_reason) {
        *err_reason = "Couldn't remove service " +
                      std::to_string(static_cast<uint32_t>(service_type));
      }
      return 0;
    }
  }

  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_) {
    connection_handler_observer_->OnServiceEndedCallback(
        session_key, service_type, CloseSessionReason::kCommon);
  }
  return session_key;
}

bool ConnectionHandlerImpl::OnSecondaryTransportStarted(
    transport_manager::ConnectionUID& primary_connection_handle,
    const transport_manager::ConnectionUID secondary_connection_handle,
    const uint8_t session_id) {
  SDL_LOG_AUTO_TRACE();

  if (session_id == 0) {
    SDL_LOG_WARN("Session id for secondary transport is invalid");
    return false;
  }

  DeviceHandle device_handle;
  Connection* connection;
  {
    sync_primitives::AutoReadLock lock(connection_list_lock_);
    ConnectionList::iterator it =
        connection_list_.find(secondary_connection_handle);
    if (connection_list_.end() == it) {
      SDL_LOG_WARN("Unknown connection " << secondary_connection_handle);
      return false;
    }

    connection = it->second;
    device_handle = connection->connection_device_handle();
  }

  // Add the secondary transport connection ID to the SessionConnectionMap
  SessionTransports st =
      SetSecondaryTransportID(session_id, secondary_connection_handle);
  primary_connection_handle = st.primary_transport;
  if (st.secondary_transport != secondary_connection_handle) {
    SDL_LOG_WARN("Failed setting the session's secondary transport ID");
    return false;
  }

  connection->SetPrimaryConnectionHandle(primary_connection_handle);

  const uint32_t session_key =
      KeyFromPair(primary_connection_handle, session_id);

  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_) {
    SDL_LOG_TRACE(
        "Calling Connection Handler Observer's "
        "OnSecondaryTransportStartedCallback");
    connection_handler_observer_->OnSecondaryTransportStartedCallback(
        device_handle, session_key);
  }

  return true;
}

void ConnectionHandlerImpl::OnSecondaryTransportEnded(
    const transport_manager::ConnectionUID primary_connection_handle,
    const transport_manager::ConnectionUID secondary_connection_handle) {
  SDL_LOG_AUTO_TRACE();

  SDL_LOG_INFO("Secondary Transport: "
               << static_cast<int32_t>(secondary_connection_handle)
               << " ended. Cleaning up services from primary connection ID "
               << static_cast<int32_t>(primary_connection_handle));
  connection_list_lock_.AcquireForReading();
  ConnectionList::iterator itr =
      connection_list_.find(primary_connection_handle);
  if (connection_list_.end() == itr) {
    SDL_LOG_ERROR("Primary Connection not found!");
    connection_list_lock_.Release();
    return;
  }
  Connection* connection = itr->second;
  connection_list_lock_.Release();

  if (connection != NULL) {
    std::list<protocol_handler::ServiceType> removed_services_list;
    uint8_t session_id = connection->RemoveSecondaryServices(
        secondary_connection_handle, removed_services_list);

    if (session_id == 0) {
      // The secondary services have already been removed from the primary
      // connection, so we find the session associated with this secondary
      // transport in the SessionConnectionMap
      session_id =
          GetSessionIdFromSecondaryTransport(secondary_connection_handle);
    }

    if (session_id != 0) {
      {
        sync_primitives::AutoReadLock read_lock(
            connection_handler_observer_lock_);
        if (connection_handler_observer_) {
          const uint32_t session_key =
              KeyFromPair(primary_connection_handle, session_id);

          // Walk the returned list of services and call the ServiceEnded
          // callback for each
          std::list<protocol_handler::ServiceType>::const_iterator it =
              removed_services_list.begin();
          for (; removed_services_list.end() != it; ++it) {
            connection_handler_observer_->OnServiceEndedCallback(
                session_key, *it, CloseSessionReason::kCommon);
          }

          connection_handler_observer_->OnSecondaryTransportEndedCallback(
              session_key);
        }
      }

      // Clear the secondary connection from the Session/Connection map entry
      // associated with this session
      SetSecondaryTransportID(session_id, 0);
    }
  }
}

const transport_manager::DeviceInfo&
ConnectionHandlerImpl::GetWebEngineDeviceInfo() const {
  return transport_manager_.GetWebEngineDeviceInfo();
}

void ConnectionHandlerImpl::CreateWebEngineDevice() {
  transport_manager_.CreateWebEngineDevice();
}

const std::string
ConnectionHandlerImpl::TransportTypeProfileStringFromConnHandle(
    transport_manager::ConnectionUID connection_handle) const {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::const_iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    SDL_LOG_WARN("Unknown connection " << connection_handle);
    return std::string();
  } else {
    DeviceHandle device_handle = it->second->connection_device_handle();
    return TransportTypeProfileStringFromDeviceHandle(device_handle);
  }
}

const std::string
ConnectionHandlerImpl::TransportTypeProfileStringFromDeviceHandle(
    DeviceHandle device_handle) const {
  std::string connection_type;
  {
    sync_primitives::AutoReadLock lock(device_list_lock_);
    auto it = device_list_.find(device_handle);
    if (device_list_.end() == it) {
      SDL_LOG_ERROR("Device not found!");
    } else {
      connection_type = it->second.connection_type();
    }
  }

  // Caution: this should be in sync with devicesType map in
  // transport_adapter_impl.cc
  if (connection_type == "USB_AOA") {
    return std::string("AOA_USB");
  } else if (connection_type == "BLUETOOTH") {
    return std::string("SPP_BLUETOOTH");
  } else if (connection_type == "USB_IOS") {
    return std::string("IAP_USB");
  } else if (connection_type == "BLUETOOTH_IOS") {
    return std::string("IAP_BLUETOOTH");
  } else if (connection_type == "WIFI") {
    return std::string("TCP_WIFI");
  } else if (connection_type == "USB_IOS_HOST_MODE") {
    return std::string("IAP_USB_HOST_MODE");
  } else if (connection_type == "USB_IOS_DEVICE_MODE") {
    return std::string("IAP_USB_DEVICE_MODE");
  } else if (connection_type == "CARPLAY_WIRELESS_IOS") {
    return std::string("IAP_CARPLAY");
  } else if (connection_type == "CLOUD_WEBSOCKET") {
    return std::string("WEBSOCKET");
  } else if (connection_type == "WEBENGINE_WEBSOCKET") {
    return std::string("WEBENGINE");
#ifdef BUILD_TESTS
  } else if (connection_type == "BTMAC") {
    return std::string("BTMAC");
#endif
  } else {
    SDL_LOG_WARN("Unknown transport type string: " << connection_type);
    return std::string();
  }
}

uint32_t ConnectionHandlerImpl::KeyFromPair(
    transport_manager::ConnectionUID connection_handle,
    uint8_t session_id) const {
  const uint32_t key = connection_handle | (session_id << 16);
  SDL_LOG_DEBUG("Key for ConnectionHandle:"
                << static_cast<uint32_t>(connection_handle)
                << " Session:" << static_cast<uint32_t>(session_id) << " is: 0x"
                << std::hex << static_cast<uint32_t>(key));
  if (protocol_handler::HASH_ID_WRONG == key) {
    SDL_LOG_ERROR(
        "Connection key is WRONG_HASH_ID "
        "(session id shall be greater 0)");
  }
  return key;
}

void ConnectionHandlerImpl::PairFromKey(
    uint32_t key,
    transport_manager::ConnectionUID* connection_handle,
    uint8_t* session_id) const {
  *connection_handle = key & 0xFF00FFFF;
  *session_id = key >> 16;
  SDL_LOG_DEBUG("ConnectionHandle: "
                << static_cast<int32_t>(*connection_handle)
                << " Session: " << static_cast<int32_t>(*session_id)
                << " for key: " << static_cast<int32_t>(key));
}

int32_t ConnectionHandlerImpl::GetDataOnSessionKey(
    uint32_t key,
    uint32_t* app_id,
    std::list<int32_t>* sessions_list,
    connection_handler::DeviceHandle* device_id) const {
  SDL_LOG_AUTO_TRACE();

  const int32_t error_result = -1;
  transport_manager::ConnectionUID conn_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &conn_handle, &session_id);

  ConnectionList::const_iterator it = connection_list_.find(conn_handle);
  if (connection_list_.end() == it) {
    SDL_LOG_ERROR("Connection not found for key: " << key);
    return error_result;
  }

  const Connection& connection = *it->second;
  const SessionMap session_map = connection.session_map();
  if (0 == session_id || session_map.end() == session_map.find(session_id)) {
    SDL_LOG_ERROR("Session not found in connection: "
                  << static_cast<int32_t>(conn_handle));
    return error_result;
  }

  if (device_id) {
    *device_id = connection.connection_device_handle();
  }
  if (app_id) {
    *app_id = KeyFromPair(conn_handle, session_id);
  }
  if (sessions_list) {
    sessions_list->clear();

    SessionMap::const_iterator session_it = session_map.begin();
    for (; session_map.end() != session_it; ++session_it) {
      sessions_list->push_back(KeyFromPair(conn_handle, it->first));
    }
  }

  SDL_LOG_INFO("Connection " << static_cast<int32_t>(conn_handle) << " has "
                             << session_map.size() << " sessions.");
  return 0;
}

const ConnectionHandlerSettings& ConnectionHandlerImpl::get_settings() const {
  return settings_;
}

const protocol_handler::SessionObserver&
ConnectionHandlerImpl::get_session_observer() {
  return *this;
}

DevicesDiscoveryStarter& ConnectionHandlerImpl::get_device_discovery_starter() {
  return *this;
}

// Finds a key not presented in std::map<unsigned char, T>
// Returns 0 if that key not found
namespace {
template <class T>
uint32_t findGap(const std::map<unsigned char, T>& map) {
  for (uint32_t i = 1; i <= UCHAR_MAX; ++i) {
    if (map.find(i) == map.end()) {
      return i;
    }
  }
  return 0;
}
}  // namespace

uint32_t ConnectionHandlerImpl::AddSession(
    const transport_manager::ConnectionUID primary_transport_id) {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoLock auto_lock(session_connection_map_lock_ptr_);
  const uint32_t session_id = findGap(session_connection_map_);
  if (session_id > 0) {
    SDL_LOG_INFO("New session ID " << session_id << " and Connection Id "
                                   << static_cast<int>(primary_transport_id)
                                   << " added to Session/Connection Map");
    SessionTransports st;
    st.primary_transport = primary_transport_id;
    st.secondary_transport = 0;
    session_connection_map_[session_id] = st;
  } else {
    SDL_LOG_WARN("Session/Connection Map could not create a new session ID!!!");
  }

  return session_id;
}

bool ConnectionHandlerImpl::RemoveSession(uint8_t session_id) {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoLock auto_lock(session_connection_map_lock_ptr_);
  SessionConnectionMap::iterator itr = session_connection_map_.find(session_id);
  if (session_connection_map_.end() == itr) {
    SDL_LOG_WARN("Session not found in Session/Connection Map!");
    return false;
  }

  SDL_LOG_INFO("Removed Session ID " << static_cast<int>(session_id)
                                     << " from Session/Connection Map");
  session_connection_map_.erase(session_id);
  return true;
}

DataAccessor<SessionConnectionMap>
ConnectionHandlerImpl::session_connection_map() {
  return DataAccessor<SessionConnectionMap>(session_connection_map_,
                                            session_connection_map_lock_ptr_);
}

SessionTransports ConnectionHandlerImpl::SetSecondaryTransportID(
    uint8_t session_id,
    transport_manager::ConnectionUID secondary_transport_id) {
  SessionTransports st;

  sync_primitives::AutoLock auto_lock(session_connection_map_lock_ptr_);
  SessionConnectionMap::iterator it = session_connection_map_.find(session_id);
  if (session_connection_map_.end() == it) {
    SDL_LOG_WARN("SetSecondaryTransportID: session ID "
                 << static_cast<int>(session_id)
                 << " not found in Session/Connection map");
    st.primary_transport = 0;
    st.secondary_transport = 0;
  } else {
    st = it->second;

    // The only time we overwrite an existing entry in the map is if the new
    // secondary transport ID is kDisabledSecondary, which effectively
    // DISABLES the secondary transport feature for the session, or if the new
    // secondary transport ID is 0, which means a secondary transport has shut
    // down
    if (st.secondary_transport != 0 &&
        secondary_transport_id != kDisabledSecondary &&
        secondary_transport_id != 0) {
      SDL_LOG_WARN("SetSecondaryTransportID: session ID "
                   << static_cast<int>(session_id)
                   << " already has a secondary connection "
                   << static_cast<int>(st.secondary_transport)
                   << " in the Session/Connection map");
    } else {
      st.secondary_transport = secondary_transport_id;
      session_connection_map_[session_id] = st;
    }
  }

  return st;
}

const SessionTransports ConnectionHandlerImpl::GetSessionTransports(
    uint8_t session_id) const {
  SessionTransports st;
  sync_primitives::AutoLock auto_lock(session_connection_map_lock_ptr_);
  SessionConnectionMap::const_iterator it =
      session_connection_map_.find(session_id);
  if (session_connection_map_.end() == it) {
    st.primary_transport = 0;
    st.secondary_transport = 0;
  } else {
    st = it->second;
  }

  return st;
}

const uint8_t ConnectionHandlerImpl::GetSessionIdFromSecondaryTransport(
    transport_manager::ConnectionUID secondary_transport_id) const {
  sync_primitives::AutoLock auto_lock(session_connection_map_lock_ptr_);
  SessionConnectionMap::const_iterator it = session_connection_map_.begin();
  for (; session_connection_map_.end() != it; ++it) {
    SessionTransports st = it->second;
    if (st.secondary_transport == secondary_transport_id) {
      return it->first;
    }
  }

  SDL_LOG_ERROR("Could not find secondary transport ID "
                << static_cast<int>(secondary_transport_id)
                << " in the Session/Connection map");
  return 0;
}

Connection* ConnectionHandlerImpl::GetPrimaryConnection(
    const ConnectionHandle connection_handle) const {
  SDL_LOG_DEBUG("Getting primary connection for ID " << connection_handle);
  ConnectionList::const_iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    SDL_LOG_ERROR("Connection with ID " << connection_handle
                                        << " was not found");
    return nullptr;
  }

  auto connection_ptr = it->second;
  if (connection_ptr->primary_connection_handle() != 0) {
    return GetPrimaryConnection(connection_ptr->primary_connection_handle());
  }

  return connection_ptr;
}

std::string ConnectionHandlerImpl::GetCloudAppID(
    const transport_manager::ConnectionUID connection_id) const {
  sync_primitives::AutoLock auto_lock(cloud_app_id_map_lock_);
  for (auto it = cloud_app_id_map_.begin(); it != cloud_app_id_map_.end();
       ++it) {
    if (connection_id == it->second.second) {
      return it->first;
    }
  }
  return std::string();
}

struct CompareMAC {
  explicit CompareMAC(const std::string& mac) : mac_(mac) {}
  bool operator()(const DeviceMap::value_type& device) {
    return strcasecmp(device.second.mac_address().c_str(), mac_.c_str()) == 0;
  }

 private:
  std::string mac_;
};

bool ConnectionHandlerImpl::GetDeviceID(const std::string& mac_address,
                                        DeviceHandle* device_handle) {
  DCHECK_OR_RETURN(device_handle, false);
  sync_primitives::AutoReadLock lock(device_list_lock_);
  DeviceMap::const_iterator it = std::find_if(
      device_list_.begin(), device_list_.end(), CompareMAC(mac_address));
  if (it != device_list_.end()) {
    *device_handle = it->first;
    return true;
  }
  return false;
}

int32_t ConnectionHandlerImpl::GetDataOnDeviceID(
    DeviceHandle device_handle,
    std::string* device_name,
    std::list<uint32_t>* applications_list,
    std::string* mac_address,
    std::string* connection_type) const {
  SDL_LOG_AUTO_TRACE();

  int32_t result = -1;

  sync_primitives::AutoReadLock lock(device_list_lock_);
  auto it = device_list_.find(device_handle);

  if (device_list_.end() == it) {
    SDL_LOG_ERROR("Device not found for handle " << device_handle);
    return result;
  }

  if (device_name) {
    *device_name = it->second.user_friendly_name();
  }
  if (connection_type) {
    *connection_type = it->second.connection_type();
  }
  if (applications_list) {
    applications_list->clear();
    sync_primitives::AutoReadLock connection_list_lock(connection_list_lock_);
    for (ConnectionList::const_iterator itr = connection_list_.begin();
         itr != connection_list_.end();
         ++itr) {
      if (device_handle == (*itr).second->connection_device_handle()) {
        const SessionMap& session_map = (itr->second)->session_map();
        for (SessionMap::const_iterator session_it = session_map.begin();
             session_map.end() != session_it;
             ++session_it) {
          const transport_manager::ConnectionUID connection_handle = itr->first;
          const uint32_t session_id = session_it->first;
          const uint32_t application_id =
              KeyFromPair(connection_handle, session_id);
          applications_list->push_back(application_id);
        }
      }
    }
  }

  if (mac_address) {
    *mac_address = it->second.mac_address();
  }

  result = 0;

  return result;
}

void ConnectionHandlerImpl::GetConnectedDevicesMAC(
    std::vector<std::string>& device_macs) const {
  DeviceMap::const_iterator first = device_list_.begin();
  DeviceMap::const_iterator last = device_list_.end();
  sync_primitives::AutoReadLock lock(device_list_lock_);

  while (first != last) {
    device_macs.push_back((*first).second.mac_address());
    ++first;
  }
}

#ifdef ENABLE_SECURITY
int ConnectionHandlerImpl::SetSSLContext(
    const uint32_t& key, security_manager::SSLContext* context) {
  SDL_LOG_AUTO_TRACE();
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  auto connection = GetPrimaryConnection(connection_handle);
  if (!connection) {
    return security_manager::SecurityManager::ERROR_INTERNAL;
  }

  return connection->SetSSLContext(session_id, context);
}

security_manager::SSLContext* ConnectionHandlerImpl::GetSSLContext(
    const uint32_t& key, const protocol_handler::ServiceType& service_type) {
  SDL_LOG_AUTO_TRACE();
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  auto connection = GetPrimaryConnection(connection_handle);
  if (!connection) {
    return nullptr;
  }

  return connection->GetSSLContext(session_id, service_type);
}

void ConnectionHandlerImpl::SetProtectionFlag(
    const uint32_t& key, const protocol_handler::ServiceType& service_type) {
  SDL_LOG_AUTO_TRACE();
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  auto connection = GetPrimaryConnection(connection_handle);
  if (!connection) {
    return;
  }

  connection->SetProtectionFlag(session_id, service_type);
}

security_manager::SSLContext::HandshakeContext
ConnectionHandlerImpl::GetHandshakeContext(uint32_t key) const {
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  auto connection = GetPrimaryConnection(connection_handle);
  if (!connection) {
    return security_manager::SSLContext::HandshakeContext();
  }

  auto primary_key = KeyFromPair(connection->connection_handle(), session_id);
  return connection_handler_observer_->GetHandshakeContext(primary_key);
}

#endif  // ENABLE_SECURITY

bool ConnectionHandlerImpl::SessionServiceExists(
    const uint32_t connection_key,
    const protocol_handler::ServiceType& service_type) const {
  SDL_LOG_AUTO_TRACE();
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::const_iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    SDL_LOG_ERROR("Unknown connection!");
    return false;
  }
  const Connection& connection = *it->second;
  return connection.SessionServiceExists(session_id, service_type);
}

void ConnectionHandlerImpl::StartDevicesDiscovery() {
  SDL_LOG_AUTO_TRACE();

  transport_manager_.SearchDevices();
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  sync_primitives::AutoReadLock lock(device_list_lock_);
  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

void ConnectionHandlerImpl::ConnectToDevice(
    connection_handler::DeviceHandle device_handle) {
  connection_handler::DeviceMap::const_iterator it_in;
  sync_primitives::AutoReadLock lock(device_list_lock_);
  it_in = device_list_.find(device_handle);
  if (device_list_.end() != it_in) {
    SDL_LOG_INFO("Connecting to device with handle " << device_handle);
    if (transport_manager::E_SUCCESS !=
        transport_manager_.ConnectDevice(device_handle)) {
      SDL_LOG_WARN("Can't connect to device");
    } else {
      SDL_LOG_ERROR(
          "Application Manager wanted to connect to non-existing device");
    }
  }
}

transport_manager::ConnectionStatus ConnectionHandlerImpl::GetConnectionStatus(
    const DeviceHandle& device_handle) const {
  return transport_manager_.GetConnectionStatus(device_handle);
}

void ConnectionHandlerImpl::RunAppOnDevice(const std::string& device_mac,
                                           const std::string& bundle_id) const {
  sync_primitives::AutoReadLock lock(device_list_lock_);
  for (DeviceMap::const_iterator i = device_list_.begin();
       i != device_list_.end();
       ++i) {
    const connection_handler::Device& device = i->second;
    if (device.mac_address() == device_mac) {
      transport_manager_.RunAppOnDevice(device.device_handle(), bundle_id);
      return;
    }
  }
  SDL_LOG_WARN("No apps found on device " << device_mac);
}

void ConnectionHandlerImpl::ConnectToAllDevices() {
  sync_primitives::AutoReadLock lock(device_list_lock_);
  for (DeviceMap::iterator i = device_list_.begin(); i != device_list_.end();
       ++i) {
    if (transport_manager::webengine_constants::kWebEngineDeviceName ==
        i->second.user_friendly_name()) {
      SDL_LOG_DEBUG("No need to connect to web engine device");
      continue;
    }
    ConnectToDevice(i->first);
  }
}

void ConnectionHandlerImpl::AddCloudAppDevice(
    const std::string& policy_app_id,
    const transport_manager::transport_adapter::CloudAppProperties&
        cloud_properties) {
  cloud_app_id_map_lock_.Acquire();
  cloud_app_id_map_[policy_app_id] =
      std::make_pair(cloud_properties.endpoint, 0);
  cloud_app_id_map_lock_.Release();
  transport_manager_.AddCloudDevice(cloud_properties);
}

void ConnectionHandlerImpl::RemoveCloudAppDevice(const DeviceHandle device_id) {
  transport_manager_.RemoveCloudDevice(device_id);
}

void ConnectionHandlerImpl::StartTransportManager() {
  SDL_LOG_AUTO_TRACE();
  transport_manager_.PerformActionOnClients(
      transport_manager::TransportAction::kVisibilityOn);
}

void ConnectionHandlerImpl::CloseRevokedConnection(uint32_t connection_key) {
  SDL_LOG_AUTO_TRACE();

  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  CloseConnection(connection_handle);
}

void ConnectionHandlerImpl::CloseConnection(
    ConnectionHandle connection_handle) {
  SDL_LOG_AUTO_TRACE();
  transport_manager::ConnectionUID connection_uid =
      ConnectionUIDFromHandle(connection_handle);
  transport_manager_.DisconnectForce(connection_uid);

  sync_primitives::AutoWriteLock connection_list_lock(connection_list_lock_);

  ConnectionList::iterator connection_list_itr =
      connection_list_.find(connection_uid);
  if (connection_list_.end() != connection_list_itr) {
    connection_list_.erase(connection_list_itr);
  }
}

uint32_t ConnectionHandlerImpl::GetConnectionSessionsCount(
    uint32_t connection_key) {
  SDL_LOG_AUTO_TRACE();
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator itr = connection_list_.find(connection_handle);

  if (connection_list_.end() != itr) {
    return itr->second->session_map().size();
  }

  return 0;
}

void ConnectionHandlerImpl::CloseSession(uint32_t key,
                                         CloseSessionReason close_reason) {
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  CloseSession(connection_handle, session_id, close_reason);
}

void ConnectionHandlerImpl::CloseSession(ConnectionHandle connection_handle,
                                         uint8_t session_id,
                                         CloseSessionReason close_reason) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Closing session with id: " << static_cast<int>(session_id));

  // In case of malformed message the connection should be broke up without
  // any other notification to mobile.
  if (close_reason != kMalformed && protocol_handler_) {
    protocol_handler_->SendEndSession(connection_handle, session_id);
  }

  transport_manager::ConnectionUID connection_id =
      ConnectionUIDFromHandle(connection_handle);

  SessionMap session_map;
  {
    sync_primitives::AutoReadLock connection_list_lock(connection_list_lock_);

    ConnectionList::iterator connection_list_itr =
        connection_list_.find(connection_id);
    if (connection_list_.end() != connection_list_itr) {
      session_map = connection_list_itr->second->session_map();
      connection_list_itr->second->RemoveSession(session_id);
    } else {
      SDL_LOG_ERROR("Connection with id: " << connection_id << " not found");
      return;
    }
  }

  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (!connection_handler_observer_) {
    SDL_LOG_ERROR("Connection handler observer not found");
    return;
  }

  SessionMap::const_iterator session_map_itr = session_map.find(session_id);
  if (session_map_itr != session_map.end()) {
    const uint32_t session_key = KeyFromPair(connection_id, session_id);
    const Session& session = session_map_itr->second;
    const ServiceList& service_list = session.service_list;

    ServiceList::const_reverse_iterator service_list_itr =
        service_list.rbegin();
    for (; service_list_itr != service_list.rend(); ++service_list_itr) {
      const protocol_handler::ServiceType service_type =
          service_list_itr->service_type;
      connection_handler_observer_->OnServiceEndedCallback(
          session_key, service_type, close_reason);
    }
  } else {
    SDL_LOG_ERROR("Session with id: " << session_id << " not found");
    return;
  }

  SDL_LOG_DEBUG("Session with id: " << session_id
                                    << " has been closed successfully");
}

void ConnectionHandlerImpl::CloseConnectionSessions(
    ConnectionHandle connection_handle, CloseSessionReason close_reason) {
  SDL_LOG_AUTO_TRACE();

  transport_manager::ConnectionUID connection_id =
      ConnectionUIDFromHandle(connection_handle);

  SDL_LOG_DEBUG(
      "Closing all sessions for connection with id: " << connection_id);

  typedef std::vector<uint8_t> SessionIdVector;
  SessionIdVector session_id_vector;
  {
    sync_primitives::AutoReadLock connection_list_lock(connection_list_lock_);

    ConnectionList::iterator connection_list_itr =
        connection_list_.find(connection_id);
    if (connection_list_.end() != connection_list_itr) {
      const SessionMap session_map = connection_list_itr->second->session_map();

      SessionMap::const_iterator session_map_itr = session_map.begin();
      for (; session_map_itr != session_map.end(); ++session_map_itr) {
        session_id_vector.push_back(session_map_itr->first);
      }
    } else {
      SDL_LOG_ERROR("Connection with id: " << connection_id << " not found");
      return;
    }
  }
  SessionIdVector::const_iterator session_id_itr = session_id_vector.begin();
  for (; session_id_itr != session_id_vector.end(); ++session_id_itr) {
    CloseSession(connection_handle, *session_id_itr, close_reason);
  }
  session_id_vector.clear();

  SDL_LOG_DEBUG("All sessions for connection with id: "
                << connection_id << " have been closed successfully");
}

void ConnectionHandlerImpl::SendEndService(uint32_t key, uint8_t service_type) {
  SDL_LOG_AUTO_TRACE();
  if (protocol_handler_) {
    uint32_t connection_handle = 0;
    uint8_t session_id = 0;
    PairFromKey(key, &connection_handle, &session_id);

    // If the service is running on a secondary transport, we need to retrieve
    // that transport from the SessionConnection Map
    SessionTransports st = GetSessionTransports(session_id);
    if (st.primary_transport == 0) {
      SDL_LOG_WARN(
          "SendEndService could not find Session in the "
          "Session/Connection Map!");
    } else {
      SDL_LOG_INFO("SendEndService found session "
                   << static_cast<int>(session_id)
                   << " with primary connection "
                   << static_cast<int>(st.primary_transport)
                   << " and secondary connection "
                   << static_cast<int>(st.secondary_transport));
      sync_primitives::AutoLock auto_lock(session_connection_map_lock_ptr_);
      auto it = std::find(st.secondary_transport_services.begin(),
                          st.secondary_transport_services.end(),
                          service_type);
      if (it != st.secondary_transport_services.end()) {
        connection_handle = st.secondary_transport;
        st.secondary_transport_services.erase(it);
      }
      protocol_handler_->SendEndService(
          st.primary_transport, connection_handle, session_id, service_type);
    }
  }
}

bool ConnectionHandlerImpl::IsSessionHeartbeatTracked(
    const uint32_t connection_key) const {
  SDL_LOG_AUTO_TRACE();
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::const_iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() != it) {
    return it->second->IsSessionHeartbeatTracked(session_id);
  }
  return false;
}

void ConnectionHandlerImpl::StartSessionHeartBeat(uint32_t connection_key) {
  SDL_LOG_AUTO_TRACE();
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() != it) {
    it->second->StartHeartBeat(session_id);
  }
}

void ConnectionHandlerImpl::SetHeartBeatTimeout(uint32_t connection_key,
                                                uint32_t timeout) {
  SDL_LOG_AUTO_TRACE();
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);
  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() != it) {
    it->second->SetHeartBeatTimeout(timeout, session_id);
  }
}

void ConnectionHandlerImpl::SendHeartBeat(ConnectionHandle connection_handle,
                                          uint8_t session_id) {
  transport_manager::ConnectionUID connection_uid =
      ConnectionUIDFromHandle(connection_handle);
  if (protocol_handler_) {
    protocol_handler_->SendHeartBeat(connection_uid, session_id);
  }
}

void ConnectionHandlerImpl::KeepConnectionAlive(uint32_t connection_key,
                                                uint8_t session_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_key);
  if (connection_list_.end() != it) {
    it->second->KeepAlive(session_id);
  }
}

void ConnectionHandlerImpl::OnConnectionEnded(
    const transport_manager::ConnectionUID connection_id) {
  SDL_LOG_INFO("Delete Connection: " << static_cast<int32_t>(connection_id)
                                     << " from the list.");
  connection_list_lock_.AcquireForWriting();
  ConnectionList::iterator itr = connection_list_.find(connection_id);
  if (connection_list_.end() == itr) {
    SDL_LOG_ERROR("Connection not found!");
    connection_list_lock_.Release();
    return;
  }
  std::unique_ptr<Connection> connection(itr->second);
  connection_list_.erase(itr);
  connection_list_lock_.Release();

  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_ && connection.get() != NULL) {
    // We have to remember the Connection object we just removed from
    // connection_list_, because we will need to retrieve the protocol
    // version from it inside of OnServiceEndedCallback
    ending_connection_ = connection.get();
    const SessionMap session_map = connection->session_map();

    const CloseSessionReason close_reason =
        connection->IsFinalMessageSent() ? CloseSessionReason::kFinalMessage
                                         : CloseSessionReason::kCommon;

    for (SessionMap::const_iterator session_it = session_map.begin();
         session_map.end() != session_it;
         ++session_it) {
      const uint32_t session_key =
          KeyFromPair(connection_id, session_it->first);
      const ServiceList& service_list = session_it->second.service_list;

      // Fix:
      // Endcallback(service_type) by Disconnected,
      // It should ended in order by 10|11 -> 7.
      // Refer to service_list.rend() of CloseSession()
      ServiceList::const_reverse_iterator service_list_itr =
          service_list.rbegin();
      for (; service_list_itr != service_list.rend(); ++service_list_itr) {
        connection_handler_observer_->OnServiceEndedCallback(
            session_key, service_list_itr->service_type, close_reason);
      }
    }
    ending_connection_ = NULL;
  }

  ConnectionHandle primary_connection_handle =
      connection->primary_connection_handle();
  if (primary_connection_handle != 0) {
    OnSecondaryTransportEnded(primary_connection_handle, connection_id);
  }
}

void ConnectionHandlerImpl::BindProtocolVersionWithSession(
    uint32_t connection_key, uint8_t protocol_version) {
  SDL_LOG_AUTO_TRACE();
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  auto connection = GetPrimaryConnection(connection_handle);
  if (connection) {
    connection->UpdateProtocolVersionSession(session_id, protocol_version);
  }
}

void ConnectionHandlerImpl::BindProtocolVersionWithSession(
    uint32_t connection_key,
    const utils::SemanticVersion& full_protocol_version) {
  SDL_LOG_AUTO_TRACE();
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  auto connection = GetPrimaryConnection(connection_handle);
  if (connection) {
    connection->UpdateProtocolVersionSession(session_id, full_protocol_version);
  }
}

bool ConnectionHandlerImpl::IsHeartBeatSupported(
    transport_manager::ConnectionUID connection_handle,
    uint8_t session_id) const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoReadLock lock(connection_list_lock_);
  const uint32_t connection_id = static_cast<uint32_t>(connection_handle);
  auto connection = GetPrimaryConnection(connection_id);

  if (!connection) {
    return false;
  }

  return connection->SupportHeartBeat(session_id);
}

bool ConnectionHandlerImpl::ProtocolVersionUsed(
    uint32_t connection_id,
    uint8_t session_id,
    uint8_t& protocol_version) const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoReadLock lock(connection_list_lock_);
  auto connection = GetPrimaryConnection(connection_id);

  if (connection) {
    return connection->ProtocolVersion(session_id, protocol_version);
  } else if (ending_connection_ &&
             static_cast<uint32_t>(ending_connection_->connection_handle()) ==
                 connection_id) {
    return ending_connection_->ProtocolVersion(session_id, protocol_version);
  }
  return false;
}

bool ConnectionHandlerImpl::ProtocolVersionUsed(
    uint32_t connection_id,
    uint8_t session_id,
    utils::SemanticVersion& full_protocol_version) const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoReadLock lock(connection_list_lock_);
  auto connection = GetPrimaryConnection(connection_id);

  if (connection) {
    return connection->ProtocolVersion(session_id, full_protocol_version);
  } else if (ending_connection_ &&
             static_cast<uint32_t>(ending_connection_->connection_handle()) ==
                 connection_id) {
    return ending_connection_->ProtocolVersion(session_id,
                                               full_protocol_version);
  }
  return false;
}

#ifdef BUILD_TESTS
ConnectionList& ConnectionHandlerImpl::getConnectionList() {
  return connection_list_;
}

const DeviceMap& ConnectionHandlerImpl::getDeviceList() {
  return device_list_;
}

void ConnectionHandlerImpl::addDeviceConnection(
    const transport_manager::DeviceInfo& device_info,
    const transport_manager::ConnectionUID connection_id) {
  // Add Device
  OnDeviceAdded(device_info);
  // Add connection
  OnConnectionEstablished(device_info, connection_id);
}
#endif  // BUILD_TESTS
}  // namespace connection_handler
