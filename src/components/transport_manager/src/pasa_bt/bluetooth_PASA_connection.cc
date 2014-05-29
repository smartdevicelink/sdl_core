/**
 * \file bluetooth_socket_connection.cc
 * \brief BluetoothPASAConnection class source file.
 *
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

#include "transport_manager/pasa_bt/bluetooth_PASA_connection.h"
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "transport_manager/pasa_bt/bluetooth_device.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

void BluetoothPASAConnection::printBufferInHex(uint8_t *buf, int len)
{
	printf("(hex)");
	for (int i=0; i<len; i++)
		printf(" %02X", buf[i]);
	printf("\n");
}


mqd_t BluetoothPASAConnection::openMsgQ(const char *queue_name, bool sender, bool block)
{
    struct mq_attr attributes;
    attributes.mq_maxmsg = MSGQ_MAX_MESSAGES;
    attributes.mq_msgsize = MAX_QUEUE_MSG_SIZE;
    attributes.mq_flags = 0;
    int flags = 0;

    if (sender)
    {
    	flags = (block) ? (O_WRONLY|O_CREAT) : (O_WRONLY|O_CREAT|O_NONBLOCK);
    }
    else
    {
    	flags = (block) ? (O_RDONLY|O_CREAT) : (O_RDONLY|O_CREAT|O_NONBLOCK);
    }

   return mq_open(queue_name, flags, 0666, &attributes);
}


BluetoothPASAConnection::BluetoothPASAConnection(
    const DeviceUID& device_uid, const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
    : read_fd_(-1), write_fd_(-1), controller_(controller),
      frames_to_send_(),
      frames_to_send_mutex_(),
      thread_(),
      terminate_flag_(false),
      unexpected_disconnect_(false),
      device_uid_(device_uid),
// Todd: BT Support - multiple SPP will be supported in the future
      sppDeviceFd(-1),
      app_handle_(app_handle) {
  pthread_mutex_init(&frames_to_send_mutex_, 0);
}
BluetoothPASAConnection::~BluetoothPASAConnection() {
    LOG4CXX_TRACE_ENTER(logger_);
    terminate_flag_ = true;
    Notify();
    pthread_join(thread_, 0);
    pthread_mutex_destroy(&frames_to_send_mutex_);

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
  return 0;
}

TransportAdapter::Error BluetoothPASAConnection::Start() {
  LOG4CXX_TRACE_ENTER(logger_);
  int fds[2];
  const int pipe_ret = pipe(fds);
  if (0 == pipe_ret) {
    LOG4CXX_INFO(logger_, "pipe created(#" << pthread_self() << ")");
    read_fd_ = fds[0];
    write_fd_ = fds[1];
  } else {
    LOG4CXX_INFO(logger_, "pipe creation failed (#" << pthread_self() << ")");
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::FAIL;
  }
  const int fcntl_ret = fcntl(read_fd_, F_SETFL,
                              fcntl(read_fd_, F_GETFL) | O_NONBLOCK);
  if (0 != fcntl_ret) {
    LOG4CXX_INFO(logger_, "fcntl failed (#" << pthread_self() << ")");
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::FAIL;
  }

  if (0 == pthread_create(&thread_, 0, &StartBluetoothPASAConnection, this)) {
    LOG4CXX_INFO(logger_, "thread created (#" << pthread_self() << ")");
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::OK;
  } else {
    LOG4CXX_INFO(logger_, "thread creation failed (#" << pthread_self() << ")");
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::FAIL;
  }
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



//  close(socket_);
  LOG4CXX_INFO(logger_, "Connection finalized");
  LOG4CXX_TRACE_EXIT(logger_);
}

TransportAdapter::Error BluetoothPASAConnection::Notify() const {
  LOG4CXX_TRACE_ENTER(logger_);
  if (-1 == write_fd_) {
    LOG4CXX_ERROR_WITH_ERRNO(
            logger_, "Failed to wake up connection thread for connection " << this);
    LOG4CXX_INFO(logger_, "exit");
    return TransportAdapter::BAD_STATE;
  }
  uint8_t c = 0;
  if (1 == write(write_fd_, &c, 1)) {
    LOG4CXX_INFO(logger_, "exit");
    return TransportAdapter::OK;
  } else {
    LOG4CXX_ERROR_WITH_ERRNO(
            logger_, "Failed to wake up connection thread for connection " << this);
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::FAIL;
  }
}

TransportAdapter::Error BluetoothPASAConnection::SendData(
    RawMessageSptr message) {
  LOG4CXX_TRACE_ENTER(logger_);
  pthread_mutex_lock(&frames_to_send_mutex_);
  frames_to_send_.push(message);
  pthread_mutex_unlock(&frames_to_send_mutex_);
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
    LOG4CXX_INFO(logger_, "Connection established (#" << pthread_self() << ")");
    controller_->ConnectDone(device_handle(), application_handle());
    while (!terminate_flag_) {
      Transmit();
    }
    LOG4CXX_INFO(logger_, "Connection is to finalize (#" << pthread_self() << ")");
    Finalize();
    while (!frames_to_send_.empty()) {
      LOG4CXX_INFO(logger_, "removing message (#" << pthread_self() << ")");
      RawMessageSptr message = frames_to_send_.front();
      frames_to_send_.pop();
      controller_->DataSendFailed(device_handle(), application_handle(),
                                  message, DataSendError());
    }
    controller_->DisconnectDone(device_handle(), application_handle());
  } else {
    LOG4CXX_INFO(logger_, "Connection Establish failed (#" << pthread_self() << ")");
    controller_->ConnectFailed(device_handle(), application_handle(),
                               *connect_error);
    delete connect_error;
  }
  LOG4CXX_TRACE_EXIT(logger_);
}

void BluetoothPASAConnection::Transmit() {
  LOG4CXX_TRACE_ENTER(logger_);
  bool pipe_notified = false;
  bool pipe_terminated = false;

  const nfds_t poll_fds_size = 2;
  pollfd poll_fds[poll_fds_size];

// Todd: BT support
  poll_fds[0].fd = sppDeviceFd;
  poll_fds[0].events = POLLIN | POLLPRI | (frames_to_send_.empty() ? 0 : POLLOUT);
  poll_fds[1].fd = read_fd_;
  poll_fds[1].events = POLLIN | POLLPRI;

  LOG4CXX_INFO(logger_, "poll (#" << pthread_self() << ") " << this);
  if (-1 == poll(poll_fds, poll_fds_size, -1)) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "poll failed for connection " << this);
    Abort();
    LOG4CXX_INFO(logger_, "exit");
    return;
  }
  LOG4CXX_INFO(logger_, "poll is ok (#" << pthread_self() << ") " << this << " revents0:" << std::hex << poll_fds[0].revents << " revents1:" << std::hex << poll_fds[1].revents);
  // error check
  if (0 != (poll_fds[1].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    LOG4CXX_ERROR(logger_,
                  "Notification pipe for connection " << this << " terminated");
    Abort();
    LOG4CXX_INFO(logger_, "exit");
    return;
  }

  if (0 != (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    LOG4CXX_INFO(logger_, "Connection " << this << " terminated");
    Abort();
    LOG4CXX_INFO(logger_, "exit");
    return;
  }

  // send data if possible
  if (!frames_to_send_.empty() && (poll_fds[0].revents | POLLOUT)) {
    LOG4CXX_INFO(logger_, "frames_to_send_ not empty()  (#" << pthread_self() << ")");
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
      LOG4CXX_INFO(logger_, "exit");
      return;
    }

    // send data
    const bool send_ok = Send();
    if (!send_ok) {
      LOG4CXX_INFO(logger_, "Send() failed  (#" << pthread_self() << ")");
      Abort();
      LOG4CXX_INFO(logger_, "exit");
      return;
    }
  }
  else
  {
// Todd: CPU logging fix - when poll_fds[1].events is set but frame is empty, then clear notification
      if( (frames_to_send_.empty()) && (poll_fds[1].revents == 1))
      {
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
            LOG4CXX_INFO(logger_, "exit");
            return;
          }
      }

  }

  // receive data
  if (0 != poll_fds[0].revents & (POLLIN | POLLPRI)) {
    const bool receive_ok = Receive();
    if (!receive_ok) {
      LOG4CXX_INFO(logger_, "Receive() failed  (#" << pthread_self() << ")");
      Abort();
      LOG4CXX_INFO(logger_, "exit");
      return;
    }
  }








  LOG4CXX_TRACE_EXIT(logger_);
}


// Todd: BT Support
bool BluetoothPASAConnection::Receive() {
  LOG4CXX_TRACE_ENTER(logger_);
  uint8_t buffer[4096];
  ssize_t bytes_read = -1;
  int32_t numBytes = 0;

  do {
//    bytes_read = recv(socket_, buffer, sizeof(buffer), MSG_DONTWAIT);
//    bytes_read = mq_receive(mhRcvQ, (char *)buffer, sizeof(buffer), 0);


        numBytes = read(sppDeviceFd, (void*)buffer, 2047);

        //sppDataSendBuffer.channelId = BT_RT_SPP_CHANNEL_1; //Hard coded the SPP channel to 1
        //sppDataSendBuffer.dataLen = numBytes;
        if (0 < numBytes)
        {
//            memcpy(&sppDataSendBuffer.data[0],sppDataBuffer,sppDataSendBuffer.dataLen );
//            sendMessage(&hBTSendQ, (UI_8*)&sppDataSendBuffer, sppDataSendBuffer.dataLen+4);
        }
        else
        {


        }


        if (numBytes > 0) {
          LOG4CXX_INFO(
              logger_,
              "Received " << numBytes << " bytes for connection " << this);

//      SPPframe* frameData = reinterpret_cast<SPPframe*>(buffer);

      //SDLLOG(ZONE_INFO, "Received Frame Packet of size " << frameData->length);
      //SDLLOG(ZONE_INFO, "Received Frame Packet: "); printBufferInHex(frameData->data, frameData->length);

          RawMessageSptr frame(
              new protocol_handler::RawMessage(0, 0, buffer, numBytes));

//      RawMessageSptr frame(
//          new protocol_handler::RawMessage(0, 0, frameData->data, static_cast<size_t>(frameData->length)));


          controller_->DataReceiveDone(device_handle(), application_handle(),
                                     frame);
        } else if (numBytes < 0) {
          if (EAGAIN != errno && EWOULDBLOCK != errno) {
            LOG4CXX_ERROR_WITH_ERRNO(logger_,
                                     "recv() failed for connection " << this);
            LOG4CXX_TRACE_EXIT(logger_);
            return false;
          }
        } else {
          LOG4CXX_INFO(logger_, "Connection " << this << " closed by remote peer");
          LOG4CXX_TRACE_EXIT(logger_);
          return false;
        }
  } while (numBytes > 0);



  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

// Todd: BT Support
bool BluetoothPASAConnection::Send() {
  LOG4CXX_TRACE_ENTER(logger_);
  FrameQueue frames_to_send;
  pthread_mutex_lock(&frames_to_send_mutex_);
  std::swap(frames_to_send, frames_to_send_);
  pthread_mutex_unlock(&frames_to_send_mutex_);

  bool frame_sent = false;
  size_t offset = 0;
  while (!frames_to_send.empty()) {
    LOG4CXX_INFO(logger_, "frames_to_send is not empty" << pthread_self() << ")");
    RawMessageSptr frame = frames_to_send.front();

    if (0 != frame)
    {
        if (( 0 != frame->data() ) &&
            ( 0u != frame->data_size() ))
        {
            //SDLLOG(ZONE_INFO, "Sending Frame Size: " << frame->mDataSize);
            //SDLLOG(ZONE_INFO, "Sending Frame Packet: "); printBufferInHex(frame->mData, frame->mDataSize);

            if (frame->data_size() < MAX_SPP_PACKET_SIZE)
            {
                SPPframe sppFrame = {};
                sppFrame.length = frame->data_size();
                memcpy(sppFrame.data, frame->data(), sppFrame.length);


                if (sppFrame.length > write(sppDeviceFd, (void*)sppFrame.data, sppFrame.length))

                //if (-1 != mq_send(mhSendQ, (const char *)&sppFrame, sizeof(sppFrame), 0))
                {
                    //SDLLOG(ZONE_INFO, "Sent " << frame->mDataSize << " bytes for connection " << ConnectionHandle);
                    frame_sent = false;
                }
                else
                {
                    //SDLLOG(ZONE_INFO, "Send failed for connection " << ConnectionHandle << " errno " << errno);
                    frame_sent = true;

                }
            }
            else
            {
                //Send Frame in MAX_SPPFRAME_MESSAGE chunks
                size_t bytesSent = 0;

                do {
                    SPPframe sppFrame = {};

                    sppFrame.length = ((bytesSent + MAX_SPP_PACKET_SIZE) < frame->data_size() )
                            ? MAX_SPP_PACKET_SIZE : frame->data_size() - bytesSent;

                    memcpy(sppFrame.data, &(frame->data())[bytesSent], sppFrame.length);

//                    if (-1 != mq_send(mhSendQ, (const char *)&sppFrame, sizeof(sppFrame), 0))
                    if (sppFrame.length > write(sppDeviceFd, (void*)sppFrame.data, sppFrame.length))

                    {
                        //SDLLOG(ZONE_INFO, "Send failed for connection " <<  ConnectionHandle);
                        frame_sent = false;
                        break;
                    }
                    else
                    {
                        bytesSent += sppFrame.length;
                        //SDLLOG(ZONE_INFO, "Sent " << bytesSent <<" out of " << frame->mDataSize << "bytes for connection " << ConnectionHandle);

                        frame_sent = true;

                    }

                } while (bytesSent < frame->data_size());
            }
        }
        else
        {
            //SDLLOG(ZONE_INFO, "Frame data is invalid for connection " << ConnectionHandle);
            frame_sent = false;
        }
    }
    else
    {
        //SDLLOG(ZONE_INFO, "Frame data is null for connection " << ConnectionHandle);
        frame_sent = false;
    }

//    const ssize_t bytes_sent = ::send(socket_, frame->data() + offset,
//                                      frame->data_size() - offset, 0);

//    if (bytes_sent >= 0) {
    if (frame_sent == true) {
      LOG4CXX_INFO(logger_, "bytes_sent >= 0" << pthread_self() << ")");
//      offset += bytes_sent;
//      if (offset == frame->data_size()) {
        frames_to_send.pop();
        offset = 0;
        controller_->DataSendDone(device_handle(), application_handle(), frame);
//      }
    } else {
//      LOG4CXX_INFO(logger_, "bytes_sent < 0" << pthread_self() << ")");
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "Send failed for connection " << this);
      frames_to_send.pop();
      offset = 0;
      controller_->DataSendFailed(device_handle(), application_handle(), frame,
                                  DataSendError());
    }
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}













bool BluetoothPASAConnection::Establish(ConnectError** error) {
  LOG4CXX_INFO(logger_, "enter (#" << pthread_self() << ")");
  DeviceSptr device = controller()->FindDevice(device_handle());

  unsigned char retry_count = 3;

  BluetoothDevice* bluetooth_device =
      static_cast<BluetoothDevice*>(device.get());

// Todd: BT Support
  if (bluetooth_device != 0)
  {
      sPPQ = bluetooth_device->GetSppQName(application_handle());

    //Open SPP device
    while (retry_count > 0)
    {
        sppDeviceFd = open(sPPQ.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (0 <= sppDeviceFd)
        {
            break;
        }
        else
        {
            retry_count--;
            LOG4CXX_ERROR_WITH_ERRNO(
                logger_,
                "Failed to open message queue " << device_handle());
        }
        delay(500);
    }

//      if ((mhSendQ != -1) && (mhRcvQ != -1))
      if(retry_count > 0)
      {
      }
      else
      {
          LOG4CXX_ERROR_WITH_ERRNO(
              logger_,
              "Failed to open message queue " << device_handle());
          *error = new ConnectError();
          LOG4CXX_INFO(logger_, "exit (#" << pthread_self() << ")");
          return false;
      }

  }

//  mq_unlink(connection->mSPPChannel.mRcvQueueName.c_str());


// Todd: BT Support
#if 0

  uint8_t rfcomm_channel;
  if (!bluetooth_device->GetRfcommChannel(application_handle(),
                                          &rfcomm_channel)) {
    LOG4CXX_ERROR(logger_,
                  "Application " << application_handle() << " not found");
    *error = new ConnectError();
    LOG4CXX_INFO(logger_, "exit (#" << pthread_self() << ")");
    return false;
  }

  struct sockaddr_rc remoteSocketAddress = { 0 };
  remoteSocketAddress.rc_family = AF_BLUETOOTH;
  memcpy(&remoteSocketAddress.rc_bdaddr, &bluetooth_device->address(),
         sizeof(bdaddr_t));
  remoteSocketAddress.rc_channel = rfcomm_channel;

  int rfcomm_socket;

  int attempts = 4;
  int connect_status = 0;
  LOG4CXX_INFO(logger_, "start rfcomm Connect attempts");
  do {
    rfcomm_socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (-1 == rfcomm_socket) {
      LOG4CXX_ERROR_WITH_ERRNO(
          logger_,
          "Failed to create RFCOMM socket for device " << device_handle());
      *error = new ConnectError();
      LOG4CXX_INFO(logger_, "exit (#" << pthread_self() << ")");
      return false;
    }
    connect_status = ::connect(rfcomm_socket,
                               (struct sockaddr*) &remoteSocketAddress,
                               sizeof(remoteSocketAddress));
    if (0 == connect_status) {
      LOG4CXX_INFO(logger_, "rfcomm Connect ok");
      break;
    }
    LOG4CXX_INFO(logger_, "rfcomm Connect errno " << errno);
    if (errno != 111 && errno != 104) {
      break;
    }
    if (errno) {
      close(rfcomm_socket);
    }
    sleep(2);
  } while (--attempts > 0);
  LOG4CXX_INFO(logger_, "rfcomm Connect attempts finished");
  if (0 != connect_status) {
    LOG4CXX_ERROR_WITH_ERRNO(
        logger_,
        "Failed to Connect to remote device " << BluetoothDevice::GetUniqueDeviceId(remoteSocketAddress.rc_bdaddr) << " for session " << this);
    *error = new ConnectError();
    LOG4CXX_INFO(logger_, "exit (#" << pthread_self() << ")");
    return false;
  }

  set_socket(rfcomm_socket);

#endif


  LOG4CXX_INFO(logger_, "exit (#" << pthread_self() << ")");
  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager

