/**
 * \file ipod_socket_connection.cc
 * \brief ipodPASAConnection class source file.
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

#include "transport_manager/ipod/ipod_PASA_connection.h"
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <sys/statvfs.h>
#include <sys/iomsg.h>

#include <sys/slog.h>
#include <sys/slogcodes.h>

#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <inttypes.h>
#include <sys/procmgr.h>

#include <ipod/ipod.h>
#include <ipod/types.h>
#include <qdb/qdb.h>


// defines
#define PULSE_CODE_EAF		(_PULSE_CODE_MINAVAIL + 1)
#define DAEMON_DEATH_CODE	(_PULSE_CODE_MINAVAIL + 2)

#define BLOCK_SIZE	('z' - '0')

// We want to send char string from 0 to z
#define			MSG_SIZE_MAX_	1000
#define 		MSG_SIZE		((MSG_SIZE_MAX_ / BLOCK_SIZE) * BLOCK_SIZE)

static char	first_message[MSG_SIZE];


#include "transport_manager/ipod/ipod_device.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

// Todd: USB iPod support



void IpodPASAConnection::printBufferInHex(uint8_t *buf, int len)
{
  printf("(hex)");
  for (int i=0; i<len; i++)
    printf(" %02X", buf[i]);
  printf("\n");
}



IpodPASAConnection::IpodPASAConnection(
    const DeviceUID& device_uid, const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
    : read_fd_(-1), write_fd_(-1), controller_(controller),
      frames_to_send_(),
      frames_to_send_mutex_(),
      thread_(),
      mEAFEventThread(),
      hdl(NULL),
      sessionid(-1),
      terminate_flag_(false),
      unexpected_disconnect_(false),
      device_uid_(device_uid),
      app_handle_(app_handle) {

  pthread_mutex_init(&frames_to_send_mutex_, 0);
}
IpodPASAConnection::~IpodPASAConnection() {
    LOG4CXX_TRACE_ENTER(logger_);
    terminate_flag_ = true;
    Notify();
    pthread_join(thread_, 0);
    pthread_join(mEAFEventThread, 0);

    pthread_mutex_destroy(&frames_to_send_mutex_);

    if (-1 != read_fd_)
      close(read_fd_);
    if (-1 != write_fd_)
      close(write_fd_);

    LOG4CXX_TRACE_EXIT(logger_);
}

void IpodPASAConnection::Abort() {
  LOG4CXX_TRACE_ENTER(logger_);
  unexpected_disconnect_ = true;
  terminate_flag_ = true;
  LOG4CXX_TRACE_EXIT(logger_);
}

void* StartIpodPASAConnection(void* v) {
  LOG4CXX_TRACE_ENTER(logger_);
  IpodPASAConnection* connection =
      static_cast<IpodPASAConnection*>(v);
  connection->Thread();
  LOG4CXX_TRACE_EXIT(logger_);
  return 0;
}

TransportAdapter::Error IpodPASAConnection::Start() {
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

  if (0 == pthread_create(&thread_, 0, &StartIpodPASAConnection, this)) {
    LOG4CXX_INFO(logger_, "thread created (#" << pthread_self() << ")");
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::OK;
  } else {
    LOG4CXX_INFO(logger_, "thread creation failed (#" << pthread_self() << ")");
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::FAIL;
  }
}

void IpodPASAConnection::Finalize() {
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

TransportAdapter::Error IpodPASAConnection::Notify() const {
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

TransportAdapter::Error IpodPASAConnection::SendData(
    RawMessagePtr message) {
  LOG4CXX_TRACE_ENTER(logger_);
  pthread_mutex_lock(&frames_to_send_mutex_);
  frames_to_send_.push(message);
  pthread_mutex_unlock(&frames_to_send_mutex_);
  LOG4CXX_TRACE_EXIT(logger_);
  return Notify();
}

TransportAdapter::Error IpodPASAConnection::Disconnect() {
  LOG4CXX_TRACE_ENTER(logger_);
  terminate_flag_ = true;
  LOG4CXX_TRACE_EXIT(logger_);
  return Notify();
}

void IpodPASAConnection::Thread() {
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
      RawMessagePtr message = frames_to_send_.front();
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

void IpodPASAConnection::Transmit() {
  LOG4CXX_TRACE_ENTER(logger_);
  bool pipe_notified = false;
  bool pipe_terminated = false;

  const nfds_t poll_fds_size = 1;
  pollfd poll_fds[poll_fds_size];

  poll_fds[0].fd = read_fd_;
  poll_fds[0].events = POLLIN | POLLPRI;

  LOG4CXX_INFO(logger_, "poll (#" << pthread_self() << ") " << this);
  if (-1 == poll(poll_fds, poll_fds_size, -1)) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "poll failed for connection " << this);
    Abort();
    LOG4CXX_INFO(logger_, "exit");
    return;
  }
  LOG4CXX_INFO(logger_, "poll is ok (#" << pthread_self() << ") " << this << " revents0:" << std::hex << poll_fds[0].revents << " revents1:" << std::hex << poll_fds[1].revents);
  // error check
  if (0 != (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    LOG4CXX_ERROR(logger_,
                  "Notification pipe for connection " << this << " terminated");
    Abort();
    LOG4CXX_INFO(logger_, "exit");
    return;
  }

  // send data if possible
  if (!frames_to_send_.empty()) {
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
// Todd: get rid of notifications to avoid CPU logging if frame is empty but notification is set thru pipe
      // clear notifications
      char buffer[256];
      ssize_t bytes_read = -1;
      do {
        bytes_read = read(read_fd_, buffer, sizeof(buffer));

      } while (bytes_read > 0);
  }

  LOG4CXX_TRACE_EXIT(logger_);
}

bool IpodPASAConnection::Send() {
  LOG4CXX_TRACE_ENTER(logger_);
  FrameQueue frames_to_send;
  pthread_mutex_lock(&frames_to_send_mutex_);
  std::swap(frames_to_send, frames_to_send_);
  pthread_mutex_unlock(&frames_to_send_mutex_);

  bool frame_sent = false;
  size_t offset = 0;
  while (!frames_to_send.empty()) {
    LOG4CXX_INFO(logger_, "frames_to_send is not empty " << pthread_self() << ")");
    RawMessagePtr frame = frames_to_send.front();

    if (0 != frame)
    {
        if (( 0 != frame->data() ) &&
            ( 0u != frame->data_size() ))
        {
            if (-1 != sendEAFData(hdl,
                                  sessionid,
                                  frame->data(),
                                  frame->data_size()))
            {
                //SDLLOG(ZONE_INFO, "Sent " << frame->mDataSize << " bytes for connection " << ConnectionHandle);

                frame_sent = true;
            }
            else
            {
                //SDLLOG(ZONE_INFO, "Send failed for connection " << ConnectionHandle << " errno " << errno);

                frame_sent = false;
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

int IpodPASAConnection::sendEAFData(ipod_hdl_t *fd, int ssid, uint8_t *buf, int len)
{
  int result = 0;
  int retry = 0;

  do
  {
    if ((result = ipod_eaf_send(fd, ssid, buf, len)) == -1)
    {
      if (errno == EIO)
      {
        delay(100);
                LOG4CXX_WARN(logger_, "Retrying to send EAF data again");
        retry++;
      }
      else
      {
                LOG4CXX_WARN(logger_, "ipod_eaf_send failure: " << strerror(errno));
        return -1;
      }
    }
    else
    {
      return 0;
    }

  }while (retry < MAX_EAF_SEND_RETRY);

  return -1;
}

void* ReceiveEAFEvents(void* arg)
{
  LOG4CXX_TRACE_ENTER(logger_);

  IpodPASAConnection* connection =
      static_cast<IpodPASAConnection*>(arg);

  connection->HandleEAFEvents();

    LOG4CXX_TRACE_EXIT(logger_);
  return (NULL);
}


#if 1
bool IpodPASAConnection::Establish(ConnectError** error) {
  LOG4CXX_INFO(logger_, "enter (#" << pthread_self() << ")");

  DeviceSptr device = controller()->FindDevice(device_handle());
  IpodDevice* ipod = static_cast<IpodDevice*>(device.get());
  bool ret = false;
  int i;
  int status;
  char				name[256];

  qdb_hdl_t* db = qdb_connect( "/dev/qdb/mediaservice_db", 0 );

  if ( db )
  {
      const char query[] = "SELECT mountpath FROM mediastores WHERE msid=%lld";
      qdb_result_t* res = qdb_query( db, 0, query, ipod->GetSrcId() );
      if ( res )
      {
          char* data = ( char* )qdb_cell( res, 0, 0 );
          if ( data ) ipod_mount_path = data;
          qdb_freeresult( res );
      }
      qdb_disconnect( db );

      hdl = ipod_connect(ipod_mount_path.c_str(), O_RDONLY);
      if (hdl != NULL) {

    //printf("hdl after con %d: \n", hdl);

          i = 0;
          do {
              // First, we get the list of registered protocols
              status = ipod_eaf_getprotocol(hdl, i, name, sizeof(name));
              if (status > 0) {
                  //printf("Protocol ID %d: %s\n", i + 1, name);
                  i++;
              }
          } while (status > 0);



          mChannelID 	  = ChannelCreate(0);
          mConnectionID = ConnectAttach(0,0,mChannelID, _NTO_SIDE_CHANNEL, 0);

          SIGEV_PULSE_INIT(&mConnectionEvent,
                           mConnectionID,
                           SIGEV_PULSE_PRIO_INHERIT,
                           DAEMON_DEATH_CODE, 0);

          if (procmgr_event_notify(PROCMGR_EVENT_DAEMON_DEATH, &mConnectionEvent) == -1) {
              LOG4CXX_ERROR_WITH_ERRNO(logger_, "Could not register with procmgr for daemon death events.  errno =  " << errno);
          }
          else
          {
              pthread_create(&mEAFEventThread, 0, &ReceiveEAFEvents, this);
              pthread_setname_np(mEAFEventThread, "EAF Event Thread");
              ret = true;
          }
      }
      else
      {
          LOG4CXX_ERROR_WITH_ERRNO(logger_, "ipod_connect failure");
      }

  }

  if(ret == false){
      *error = new ConnectError();
  }

  LOG4CXX_INFO(logger_, "exit (#" << pthread_self() << ")");
  return ret;
}

#endif

void IpodPASAConnection::HandleEAFEvents() {
  LOG4CXX_TRACE_ENTER(logger_);

  do
  {
      SIGEV_PULSE_INIT(&mConnectionEvent,
                       mConnectionID,
                       SIGEV_PULSE_PRIO_INHERIT,
                       PULSE_CODE_EAF, 0);


//printf("hdl handleEafevents %d: \n", hdl);


      if (ipod_notify(hdl, _NOTIFY_ACTION_POLLARM, _NOTIFY_COND_OBAND, &mConnectionEvent) == -1) {
          if (errno == EBADF) {
              // iPod driver is slain
              LOG4CXX_ERROR_WITH_ERRNO(logger_, "iPod ejected: " << errno);
              return;
          } else {
              LOG4CXX_ERROR_WITH_ERRNO(logger_, "Error registering for notification: " << errno);
          }
          break;
      }

      mConnectionPulse.code = PULSE_CODE_EAF;

      switch(mConnectionPulse.code)
      {
          case PULSE_CODE_EAF:
          {
              ipod_eaf_event_t events[20];
              int iNoOfEvents  = 0;

              do
              {
                  if ((iNoOfEvents = ipod_eaf_getevents(hdl, events, 20)) == -1)
                  {
                      LOG4CXX_ERROR_WITH_ERRNO(logger_, "ipod_eaf_getevents failure (" << strerror(errno) << ")");
                  }
                  else
                  {

//printf("eaf event# %d: \n", iNoOfEvents);

                      for (int i=0; i<iNoOfEvents; i++)
                      {
                          switch (events[i].eventtype)
                          {
                              case IPOD_EAF_EVENT_SESSION_REQ:
                              {
                                  LOG4CXX_INFO(logger_, "IPOD_EAF_EVENT_SESSION_REQ - protid " << events[i].eventinfo);

                                  if (ipod_eaf_session_accept(hdl, events[i].eventinfo, 0) == -1)
                                  {
                                      LOG4CXX_ERROR_WITH_ERRNO(logger_, "ipod_eaf_session_accept failure: " << strerror(errno));
                                  }
                              }
                              break;

                              case IPOD_EAF_EVENT_SESSION_CLOSE:
                              {
                                  LOG4CXX_INFO(logger_, "IPOD_EAF_EVENT_SESSION_CLOSE - ssid " << events[i].eventinfo);

                                  // to protect invalid data failure (upon startup)
                                  if (sessionid != -1)
                                  {
                                      if (ipod_eaf_session_free(hdl, events[i].eventinfo) == -1)
                                      {
                                          LOG4CXX_ERROR_WITH_ERRNO(logger_, "ipod_eaf_session_free failure: " << strerror(errno));
                                      }
                                  }
                              }
                              break;

                              case IPOD_EAF_EVENT_SESSION_DATA:
                              {
                                  if (sessionid != -1)
                                  {
                                      LOG4CXX_INFO(logger_, "IPOD_EAF_EVENT_SESSION_DATA - ssid " << events[i].eventinfo);

                                      uint8_t frameData[MAX_EAF_BUFF_SIZE];
                                      int frameSize = ipod_eaf_recv(hdl,
                                                                    events[i].eventinfo,
                                                                    frameData,
                                                                    MAX_EAF_BUFF_SIZE);

                                      if (frameSize > 0)
                                      {
                                          //SDLLOG(ZONE_INFO, "Received Frame Packet of size " << frameData->length);
                                          //SDLLOG(ZONE_INFO, "Received Frame Packet: "); printBufferInHex(frameData->data, frameData->length);

                                          RawMessagePtr frame(
                                              new protocol_handler::RawMessage(0, 0, frameData, static_cast<size_t>(frameSize)));
                                          controller_->DataReceiveDone(device_handle(), application_handle(),
                                                                         frame);
                                          LOG4CXX_INFO(logger_, "Received Frame Packet of size " << frameSize);

                                          //SDLLOG(ZONE_INFO, "Received Frame Packet: "); CAdapter->printBufferInHex(frameData, frameSize);

                                      }
                                  }
                              }
                              break;

                              case IPOD_EAF_EVENT_SESSION_OPEN:
                              {
                                  ///TODO: Support more than one protocolID - eaf.ssid[events[index].eventinfo]
                                  LOG4CXX_INFO(logger_, "IPOD_EAF_EVENT_SESSION_OPEN - protid " << events[i].eventinfo);

                                  sessionid = ipod_eaf_session_open(hdl, events[i].eventinfo);
                                  if (sessionid < 0)
                                  {
                                      LOG4CXX_ERROR_WITH_ERRNO(logger_, "ipod_eaf_session_open failure: " << strerror(errno));
                                  }
                              }
                              break;

                              default:
                              {
                                  LOG4CXX_WARN(logger_, "Unknown EAF Event Type " << (int)events[i].eventtype);
                              }
                              break;
                          }
                      }
                  }
              } while (iNoOfEvents == 20);
          }
          break;

          case DAEMON_DEATH_CODE:
          {
              if (access(ipod_mount_path.c_str(), F_OK) != 0) {
                  LOG4CXX_WARN(logger_, "fstat error, assuming iPod driver slain");
              }
          }
          break;

          default:
          {
              LOG4CXX_WARN(logger_, "Unknown PULSE CODE (" << (int)mConnectionPulse.code << ")");
          }
          break;

      }

      //Blocking call MsgReceivePulse
      if (MsgReceivePulse(mChannelID, &mConnectionPulse, sizeof(mConnectionPulse), NULL) == -1)
      {
          LOG4CXX_ERROR_WITH_ERRNO(logger_, "Error receiving pulse");
          break;
      }
  }while (!terminate_flag_);

  LOG4CXX_TRACE_EXIT(logger_);
}






}  // namespace transport_adapter
}  // namespace transport_manager

