/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_VR_PROXY_H_
#define SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_VR_PROXY_H_

#include <queue>

#include "utils/lock.h"
#include "utils/macro.h"
#include "utils/threads/message_loop_thread.h"
#include "vr_module/interface/hmi.pb.h"

namespace threads {
class Thread;
}  // namespace threads

namespace vr_module {

typedef std::queue<vr_hmi_api::ServiceMessage> MessageQueue;

class VRProxyListener;
class Channel;

class VRProxy : public threads::MessageLoopThread<MessageQueue>::Handler {
 public:
  explicit VRProxy(VRProxyListener *listener);
  VRProxy(VRProxyListener *listener, Channel *channel);
  ~VRProxy();

  /**
   * Starts channel to connect to HMI(Applink)
   */
  bool Start();

  /**
   * Stops channel to HMI(Applink)
   */
  void Stop();

  /**
   * Sends message to HMI(Applink)
   * @param message to send
   * @return true if success
   */
  bool Send(const vr_hmi_api::ServiceMessage& message);

 private:
  /**
   * Create thread to receive data from HMI(Applink)
   */
  inline void CreateThread();

  /**
   * Routine to receive messages from HMI(Applink)
   */
  void Receive();

  /**
   * Handles message from queue from HMI(Applink)
   * @param message received message
   */
  void Handle(vr_hmi_api::ServiceMessage message);

  /**
   * Handles that channel to HMI is established successfully.
   */
  void OnEstablished();

  /**
   * Handles receiving message from channel
   * @param message received message
   */
  void OnReceived(const vr_hmi_api::ServiceMessage& message);

  /**
   * Converts size of message to string
   * @param value integer presentaiont of size
   * @return string presentation of size
   */
  std::string SizeToString(uint32_t value);

  /**
   * Converts string to size of message
   * @param value string presentaiont of size
   * @return integer presentaiont of size
   */
  uint32_t SizeFromString(const std::string& value);
  VRProxyListener *listener_;
  threads::MessageLoopThread<MessageQueue> incoming_;
  Channel *channel_;
  threads::Thread *channel_thread_;
  friend class Receiver;
  FRIEND_TEST(VRProxyTest, SizeToString);
  FRIEND_TEST(VRProxyTest, SizeFromString);
  FRIEND_TEST(VRProxyTest, Receive);
};

}  // namespace vr_module

#endif  // SRC_COMPONENTS_VR_MODULE_INCLUDE_VR_MODULE_VR_PROXY_H_
