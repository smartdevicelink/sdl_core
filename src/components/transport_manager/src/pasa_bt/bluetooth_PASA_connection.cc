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

#include "transport_manager/pasa_bt/bluetooth_PASA_connection.h"
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "transport_manager/pasa_bt/bluetooth_PASA_device.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

#define MAX_SPP_PACKET_SIZE    2047

typedef struct _SPPframe {
  uint16_t length;
  uint8_t  data[MAX_SPP_PACKET_SIZE];
} SPPframe;

BluetoothPASAConnection::BluetoothPASAConnection(
    const DeviceUID& device_uid, const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
    : read_fd_(-1), write_fd_(-1), controller_(controller),
      thread_(),
      terminate_flag_(false),
      unexpected_disconnect_(false),
      device_uid_(device_uid),
      sppDeviceFd(-1),
      app_handle_(app_handle) {
}
BluetoothPASAConnection::~BluetoothPASAConnection() {
    LOG4CXX_TRACE_ENTER(logger_);
    terminate_flag_ = true;
    Notify();
    pthread_join(thread_, 0);
    if (-1 != read_fd_)
      close(read_fd_);
    if (-1 != write_fd_)
      close(write_fd_);
    LOG4CXX_TRACE_EXIT(logger_);
}

void BluetoothPASAConnection::Abort() {
  LOG4CXX_TRACE_ENTER(logger_);
  unexpected_disconnect_ = true;
  terminate_flag_ = true;
  LOG4CXX_TRACE_EXIT(logger_);
}

void* StartBluetoothPASAConnection(void* v) {
  LOG4CXX_TRACE_ENTER(logger_);
  BluetoothPASAConnection* connection =
      static_cast<BluetoothPASAConnection*>(v);
  connection->Thread();
  LOG4CXX_TRACE_EXIT(logger_);
  return NULL;
}

TransportAdapter::Error BluetoothPASAConnection::Start() {
  LOG4CXX_TRACE_ENTER(logger_);
  int fds[2];
  const int pipe_ret = pipe(fds);
  if (0 != pipe_ret) {
    LOG4CXX_ERROR(logger_, "pipe creation failed (#" << pthread_self() << ")");
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::FAIL;
  }
  LOG4CXX_DEBUG(logger_, "pipe created (#" << pthread_self() << ")");
  read_fd_  = fds[0];
  write_fd_ = fds[1];
  const int fcntl_ret = fcntl(read_fd_, F_SETFL,
                              fcntl(read_fd_, F_GETFL) | O_NONBLOCK);
  if (0 != fcntl_ret) {
    LOG4CXX_ERROR(logger_, "fcntl failed (#" << pthread_self() << ")");
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::FAIL;
  }
  if (0 != pthread_create(&thread_, 0, &StartBluetoothPASAConnection, this)) {
    LOG4CXX_ERROR(logger_, "thread creation failed (#" << pthread_self() << ")");
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::FAIL;
  }
  LOG4CXX_DEBUG(logger_, "thread created (#" << pthread_self() << ")");
  LOG4CXX_TRACE_EXIT(logger_);
  return TransportAdapter::OK;
}

void BluetoothPASAConnection::Finalize() {
  LOG4CXX_TRACE_ENTER(logger_);
  if (unexpected_disconnect_) {
    LOG4CXX_INFO(logger_, "unexpected_disconnect (#" << pthread_self() << ")");
    controller_->ConnectionAborted(device_handle(), application_handle(),
                                   CommunicationError());
  } else {
    LOG4CXX_INFO(logger_, "not unexpected_disconnect (#" << pthread_self() << ")");
    controller_->ConnectionFinished(device_handle(), application_handle());
  }
  LOG4CXX_INFO(logger_, "Connection finalized");
  LOG4CXX_TRACE_EXIT(logger_);
}

TransportAdapter::Error BluetoothPASAConnection::Notify() const {
  LOG4CXX_TRACE_ENTER(logger_);
  if (-1 == write_fd_) {
    LOG4CXX_ERROR_WITH_ERRNO(
            logger_, "Failed to wake up connection thread for connection " << this);
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::BAD_STATE;
  }
  uint8_t c = 0;
  if (1 != write(write_fd_, &c, 1)) {
    LOG4CXX_ERROR_WITH_ERRNO(
            logger_, "Failed to wake up connection thread for connection " << this);
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::FAIL;
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return TransportAdapter::OK;
}

TransportAdapter::Error BluetoothPASAConnection::SendData(
    RawMessageSptr message) {
  LOG4CXX_TRACE_ENTER(logger_);
  sync_primitives::AutoLock auto_lock(frames_to_send_lock_);
  frames_to_send_.push(message);
  LOG4CXX_TRACE_EXIT(logger_);
  return Notify();
}

TransportAdapter::Error BluetoothPASAConnection::Disconnect() {
  LOG4CXX_TRACE_ENTER(logger_);
  terminate_flag_ = true;
  LOG4CXX_TRACE_EXIT(logger_);
  return Notify();
}

void BluetoothPASAConnection::Thread() {
  LOG4CXX_TRACE_ENTER(logger_);
  controller_->ConnectionCreated(this, device_uid_, app_handle_);
  ConnectError* connect_error = NULL;
  if (Establish(&connect_error)) {
    LOG4CXX_DEBUG(logger_, "Connection established (#" << pthread_self() << ")");
    controller_->ConnectDone(device_handle(), application_handle());
    while (!terminate_flag_) {
      Transmit();
    }
    LOG4CXX_DEBUG(logger_, "Connection is to finalize (#" << pthread_self() << ")");
    Finalize();
    sync_primitives::AutoLock auto_lock(frames_to_send_lock_);
    while (!frames_to_send_.empty()) {
      LOG4CXX_DEBUG(logger_, "removing message (#" << pthread_self() << ")");
      RawMessageSptr message = frames_to_send_.front();
      frames_to_send_.pop();
      controller_->DataSendFailed(device_handle(), application_handle(),
                                  message, DataSendError());
    }
    controller_->DisconnectDone(device_handle(), application_handle());
  } else {
    LOG4CXX_ERROR(logger_, "Connection Establish failed (#" << pthread_self() << ")");
    controller_->ConnectFailed(device_handle(), application_handle(),
                               *connect_error);
    delete connect_error;
  }
  LOG4CXX_TRACE_EXIT(logger_);
}

void BluetoothPASAConnection::Transmit() {
  LOG4CXX_TRACE_ENTER(logger_);

  const nfds_t poll_fds_size = 2;
  pollfd poll_fds[poll_fds_size];

  poll_fds[0].fd = sppDeviceFd;
  poll_fds[0].events = POLLIN | POLLPRI | (frames_to_send_.empty() ? 0 : POLLOUT);
  poll_fds[1].fd = read_fd_;
  poll_fds[1].events = POLLIN | POLLPRI;

  LOG4CXX_DEBUG(logger_, "poll (#" << pthread_self() << ") " << this);
  if (-1 == poll(poll_fds, poll_fds_size, -1)) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "poll failed for connection " << this);
    Abort();
    LOG4CXX_TRACE_EXIT(logger_);
    return;
  }
  LOG4CXX_DEBUG(logger_, "poll is ok (#" << pthread_self() << ") " << this
               << " revents0:" << std::hex << poll_fds[0].revents
               << " revents1:" << std::hex << poll_fds[1].revents);
  // error check
  if (0 != (poll_fds[1].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    LOG4CXX_WARN(logger_,
                  "Notification pipe for connection " << this << " terminated");
    Abort();
    LOG4CXX_TRACE_EXIT(logger_);
    return;
  }

  if (0 != (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    LOG4CXX_WARN(logger_, "Connection " << this << " terminated");
    Abort();
    LOG4CXX_TRACE_EXIT(logger_);
    return;
  }

  // send data if possible
  if (!frames_to_send_.empty() && (poll_fds[0].revents | POLLOUT)) {
    LOG4CXX_DEBUG(logger_, "frames_to_send_ not empty()  (#" << pthread_self() << ")");
    // clear notifications
    char buffer[256];
    ssize_t bytes_read = -1;
    do {
      bytes_read = read(read_fd_, buffer, sizeof(buffer));
    } while (bytes_read > 0);

    if ((bytes_read < 0) && (EAGAIN != errno)) {
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to clear notification pipe");
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "poll failed for connection " << this);
      Abort();
      LOG4CXX_TRACE_EXIT(logger_);
      return;
    }

    // send data
    if (!Send()) {
      LOG4CXX_ERROR(logger_, "Send() failed  (#" << pthread_self() << ")");
      Abort();
      LOG4CXX_TRACE_EXIT(logger_);
      return;
    }
  } else {
    // Todd: CPU logging fix - when poll_fds[1].events is set but frame is empty, then clear notification
    if( (frames_to_send_.empty()) && (poll_fds[1].revents == 1)) {
      // clear notifications
      char buffer[256];
      ssize_t bytes_read = -1;
      do {
        bytes_read = read(read_fd_, buffer, sizeof(buffer));
      } while (bytes_read > 0);

      if ((bytes_read < 0) && (EAGAIN != errno)) {
        LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to clear notification pipe");
        LOG4CXX_ERROR_WITH_ERRNO(logger_, "poll failed for connection " << this);
        Abort();
        LOG4CXX_TRACE_EXIT(logger_);
        return;
      }
    }
  }
  // receive data
  if (0 != poll_fds[0].revents & (POLLIN | POLLPRI)) {
    if (!Receive()) {
      LOG4CXX_ERROR(logger_, "Receive() failed  (#" << pthread_self() << ")");
      Abort();
      LOG4CXX_TRACE_EXIT(logger_);
      return;
    }
  }
  LOG4CXX_TRACE_EXIT(logger_);
}

bool BluetoothPASAConnection::Receive() {
  LOG4CXX_TRACE_ENTER(logger_);
  uint8_t buffer[4096];
  int32_t numBytes = 0;

  do {
    numBytes = read(sppDeviceFd, (void*)buffer, 2047);
    if (numBytes > 0) {
      LOG4CXX_DEBUG( logger_,
            "Received " << numBytes << " bytes for connection " << this);
      RawMessageSptr frame(
            new protocol_handler::RawMessage(0, 0, buffer, numBytes));
      controller_->DataReceiveDone(device_handle(), application_handle(), frame);
    } else if (numBytes < 0) {
      if (EAGAIN != errno && EWOULDBLOCK != errno) {
        LOG4CXX_ERROR_WITH_ERRNO(logger_,
                                 "recv() failed for connection " << this);
        LOG4CXX_TRACE_EXIT(logger_);
        return false;
      }
    } else {
      LOG4CXX_WARN(logger_, "Connection " << this << " closed by remote peer");
      LOG4CXX_TRACE_EXIT(logger_);
      return false;
    }
  } while (numBytes > 0);
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

bool BluetoothPASAConnection::Send() {
  LOG4CXX_TRACE_ENTER(logger_);
  FrameQueue frames_to_send;
  frames_to_send_lock_.Acquire();
  std::swap(frames_to_send, frames_to_send_);
  frames_to_send_lock_.Release();

  while (!frames_to_send.empty()) {
    LOG4CXX_INFO(logger_, "frames_to_send is not empty" << pthread_self() << ")");
    RawMessageSptr frame = frames_to_send.front();
    bool frame_sent = false;
    if (frame) {
      if (frame->data() && frame->data_size() > 0 ) {
        if (frame->data_size() < MAX_SPP_PACKET_SIZE) {
          SPPframe sppFrame = {};
          sppFrame.length = frame->data_size();
          memcpy(sppFrame.data, frame->data(), sppFrame.length);
          const ssize_t written = write(sppDeviceFd, (void*)sppFrame.data, sppFrame.length);
          frame_sent = (written >= sppFrame.length);
        } else {
          //Send Frame in MAX_SPPFRAME_MESSAGE chunks
          size_t bytesSent = 0;
          do {
            SPPframe sppFrame = {};
            sppFrame.length = ((bytesSent + MAX_SPP_PACKET_SIZE) < frame->data_size() )
                ? MAX_SPP_PACKET_SIZE : frame->data_size() - bytesSent;

            memcpy(sppFrame.data, &(frame->data())[bytesSent], sppFrame.length);

            const ssize_t written = write(sppDeviceFd, (void*)sppFrame.data, sppFrame.length);
            frame_sent = (written >= sppFrame.length);

            if (!frame_sent){
              break;
            }
            bytesSent += sppFrame.length;
          } while (bytesSent < frame->data_size());
        }
      }
    }
    if (frame_sent) {
      LOG4CXX_INFO(logger_, "bytes_sent >= 0" << pthread_self() << ")");
      controller_->DataSendDone(device_handle(), application_handle(), frame);
    } else {
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "Send failed for connection " << this);
      controller_->DataSendFailed(device_handle(), application_handle(), frame,
                                  DataSendError());
    }
    frames_to_send.pop();
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

bool BluetoothPASAConnection::Establish(ConnectError** error) {
  LOG4CXX_INFO(logger_, "enter (#" << pthread_self() << ")");
  DeviceSptr device = controller()->FindDevice(device_handle());

  unsigned char retry_count = 3;

  BluetoothPASADevice* bluetooth_device =
      static_cast<BluetoothPASADevice*>(device.get());

  if (bluetooth_device != 0) {
    sPPQ = bluetooth_device->GetSppQName(application_handle());

    //Open SPP device
    while (retry_count > 0) {
      sppDeviceFd = open(sPPQ.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
      //on error open
      if (-1 == sppDeviceFd){
        LOG4CXX_ERROR_WITH_ERRNO( logger_,
                                  "Failed to open message queue " << device_handle());
        if(retry_count-- == 0) {
          if(error) {
            *error = new ConnectError();
          }
          LOG4CXX_INFO(logger_, "exit (#" << pthread_self() << ")");
          return false;
        }
        delay(500);
      }
    }
  }
  LOG4CXX_INFO(logger_, "exit (#" << pthread_self() << ")");
  return true;
}

TransportAdapterController* BluetoothPASAConnection::controller() {
  return controller_;
}

DeviceUID BluetoothPASAConnection::device_handle() const {
  return device_uid_;
}

ApplicationHandle BluetoothPASAConnection::application_handle() const {
  return app_handle_;
}

}  // namespace transport_adapter
}  // namespace transport_manager
