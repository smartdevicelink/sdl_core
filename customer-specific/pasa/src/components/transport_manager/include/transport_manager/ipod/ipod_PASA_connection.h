/**
 * \file ipod_socket_connection.h
 * \brief ipodPASAConnection class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_IPOD_IPOD_SOCKET_CONNECTION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_IPOD_IPOD_SOCKET_CONNECTION_H_

#include <poll.h>

#include <queue>
#include <sys/siginfo.h>
#include <sys/neutrino.h>

#include "utils/logger.h"
#include "transport_manager/transport_adapter/connection.h"
#include <ipod/ipod.h>
#include <ipod/types.h>

#define MAX_QUEUE_NAME_SIZE     24
#define MAX_QUEUE_MSG_SIZE 		4095
#define MSGQ_MAX_MESSAGES		128
#define MSGQ_MESSAGE_SIZE 		MAX_QUEUE_MSG_SIZE

#define MME_QDB_NAME	"/dev/qdb/mme"
#define IPOD_EAF_PATH	"/.FS_info./eaf"
#define IPOD_CTRL_PATH	"/.FS_info./control"
#define IPOD_EVT_PATH	"/.FS_info./events"
#define IPOD_AUDIO_PATH	"/.FS_info./audio"

#define PULSE_CODE_EAF      (_PULSE_CODE_MINAVAIL + 1)
#define DAEMON_DEATH_CODE   (_PULSE_CODE_MINAVAIL + 2)

#define MAX_EAF_BUFF_SIZE   32768
#define MAX_EAF_SEND_RETRY	3


using ::transport_manager::transport_adapter::Connection;

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Class responsible for communication over ipod sockets.
 */
class IpodPASAConnection : public Connection {
 public:

  /**
   * @brief Constructor.
   *
   * @param device_uid Device unique identifier.
   * @param app_handle Handle of device.
   * @param controller Pointer to the device adapter controller.
   */
  IpodPASAConnection(const DeviceUID& device_uid,
                            const ApplicationHandle& app_handle,
                            TransportAdapterController* controller);


  /**
   * @brief Send data frame.
   *
   * @param Message Smart pointer to the raw message.
   *
   * @return Error Information about possible reason of sending data failure.
   */
  TransportAdapter::Error SendData(RawMessagePtr message);

  /**
   * @brief Disconnect the current connection.
   *
   * @return Error Information about possible reason of Disconnect failure.
   */
  TransportAdapter::Error Disconnect();

  /**
   * @brief Start thread creation.
   *
   * @return Information about possible reason of thread creation error.
   */
  TransportAdapter::Error Start();

  /**
   * @brief Destructor.
   */
  virtual ~IpodPASAConnection();

protected:

  /**
   * @brief Establish connection.
   *
   * @param error Connection error.
   *
   * @return True - connection established.
   * false - connection not established.
   */
  virtual bool Establish(ConnectError** error);


  /**
   * @brief Return pointer to the device adapter controller.
   */
  TransportAdapterController* controller() {
    return controller_;
  }

  /**
   * @brief Return device unique identifier.
   */
  DeviceUID device_handle() const {
    return device_uid_;
  }

  /**
   * @brief Return handle of application.
   */
  ApplicationHandle application_handle() const {
    return app_handle_;
  }


 private:

  int read_fd_;
  int write_fd_;
  void Thread();
  void HandleEAFEvents();

//int handle_eaf_events(ipod_hdl_t *hdl);

  void Transmit();
  void Finalize();
  TransportAdapter::Error Notify() const;
  bool Send();
  void Abort();
  void printBufferInHex(uint8_t *buf, int len);

  friend void* ReceiveEAFEvents(void* arg);

  int sendEAFData(ipod_hdl_t *fd, int ssid, uint8_t *buf, int len);

  friend void* StartIpodPASAConnection(void*);

  /**
   * @brief Frames that must be sent to remote device.
   **/
  typedef std::queue<RawMessagePtr> FrameQueue;
  FrameQueue frames_to_send_;
  mutable pthread_mutex_t frames_to_send_mutex_;

  pthread_t thread_;

  ipod_hdl_t *hdl;
  std::string ipod_mount_path;

  int sessionid;

  int mChannelID;
  int mConnectionID;

  struct _pulse mConnectionPulse;
  struct sigevent mConnectionEvent;
  TransportAdapterController* controller_;

  bool terminate_flag_;
  bool unexpected_disconnect_;
  const DeviceUID device_uid_;
  const ApplicationHandle app_handle_;
  pthread_t mEAFEventThread;

};

extern log4cxx::LoggerPtr logger_;

}  // namespace transport_adapter
}  // namespace transport_manager

#endif /* IPOD_SOCKET_CONNECTION_H_ */
