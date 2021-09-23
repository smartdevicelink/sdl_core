/*
 *
 * Copyright (c) 2021, Ford Motor Company
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

#include "transport_manager/android_ipc/android_socket_connection.h"

#include "transport_manager/android_ipc/android_ipc_device.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/android_ipc/android_transport_adapter.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {
SDL_CREATE_LOG_VARIABLE("TransportManager")

AndroidSocketConnection::AndroidSocketConnection(
    const DeviceUID& device_uid,
    const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
    : Connection()
    , device_uid_(device_uid)
    , app_handle_(app_handle)
    , controller_(controller)
    , sender_(std::make_shared<LocalSocketSender>(std::bind(&AndroidSocketConnection::OnMessageSent, this, std::placeholders::_1),
                  std::bind(&AndroidSocketConnection::OnClientConnectionDone, this, std::placeholders::_1)))
    , receiver_(std::make_shared<LocalSocketReceiver>(
                    std::bind(&AndroidSocketConnection::ProcessMessage, this, std::placeholders::_1)))
    {}

AndroidSocketConnection::~AndroidSocketConnection() {}

void AndroidSocketConnection::ProcessMessage(const std::vector<uint8_t>& data) {
    if (data.size() > 0) {
        SDL_LOG_DEBUG("Received " << data.size() << " bytes from Java adapter");
        ::protocol_handler::RawMessagePtr frame(
            new protocol_handler::RawMessage(0, 0, data.data(), data.size(), false));
        controller_->DataReceiveDone(device_uid_, app_handle_, frame);
    } else {
        controller_->DataReceiveFailed(device_uid_, app_handle_, DataReceiveError());
    }
}

void AndroidSocketConnection::OnMessageSent(protocol_handler::RawMessagePtr msg) {
    if (msg) {
        SDL_LOG_DEBUG("Successfully sent " << msg->data_size() << " bytes to Java adapter");
        controller_->DataSendDone(device_uid_, app_handle_, msg);
    } else {
        SDL_LOG_ERROR("Failed to send " << msg->data_size() << " bytes to Java adapter");
        controller_->DataSendFailed(device_uid_, app_handle_, msg, DataSendError());
    }
}

void AndroidSocketConnection::OnClientConnectionDone(const bool is_connected) {
    if (is_connected) {
        SDL_LOG_DEBUG("Successfully connected to Java adapter");
        controller_->ConnectDone(device_uid_, app_handle_);
    } else {
        SDL_LOG_ERROR("Failed to connect to Java adapter");
        controller_->ConnectFailed(device_uid_, app_handle_, ConnectError());
    }
}

TransportAdapter::Error
AndroidSocketConnection::SendData(::protocol_handler::RawMessagePtr message) {
    SDL_LOG_AUTO_TRACE();
    sender_->Send(message);
    return TransportAdapter::OK;
}

TransportAdapter::Error AndroidSocketConnection::Disconnect() {
    SDL_LOG_DEBUG("Disconnecting from Java adapter");
    sender_->Stop();
    if (sender_thread_.joinable()) {
        sender_thread_.join();
    }

    receiver_->Stop();
    if (receiver_thread_.joinable()) {
        receiver_thread_.join();
    }

    return TransportAdapter::OK;
}

TransportAdapter::Error AndroidSocketConnection::Start() {
    SDL_LOG_DEBUG("Initializing Android IPC connection threads");

    receiver_thread_ = std::thread([&]() {
        receiver_->Init(static_cast<AndroidTransportAdapter*>(controller_)->GetReceiverSocketName());
        receiver_->Run();
    });
    sender_thread_ = std::thread([&]() {
        sender_->Init(static_cast<AndroidTransportAdapter*>(controller_)->GetSenderSocketName());
        sender_->Run();
    });

    return TransportAdapter::OK;
}

}  // namespace transport_adapter
}  // namespace transport_manager
