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

#include <pthread.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "vr_module/vr_proxy.h"
#include "mock_vr_proxy_listener.h"
#include "mock_channel.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;

MATCHER_P(ServiceMessageEq, expected, "") {
  return arg.rpc() == expected.rpc()
      && arg.rpc_type() == expected.rpc_type()
      && arg.correlation_id() == expected.correlation_id()
      && (arg.has_params() == expected.has_params()
          || arg.params() == expected.params());
}

ACTION_P(Unlock, mutex) {
  pthread_mutex_unlock(mutex);
}

namespace vr_module {

class VRProxyTest : public ::testing::Test {
 protected:
};

TEST_F(VRProxyTest, StartStop) {
  MockVRProxyListener listener;
  MockChannel *channel = new MockChannel();
  EXPECT_CALL(*channel, Start()).Times(1).WillOnce(Return(true));
  EXPECT_CALL(listener, OnReady()).Times(1);
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  EXPECT_CALL(*channel, Receive(_, _)).WillRepeatedly(
      DoAll(Unlock(&mutex), Return(false)));
  EXPECT_CALL(*channel, Stop()).Times(AtLeast(1)).WillRepeatedly(Return(true));
  VRProxy proxy(&listener, channel);
  pthread_mutex_lock(&mutex);
  EXPECT_TRUE(proxy.Start());
  pthread_mutex_lock(&mutex);
  proxy.Stop();
}

TEST_F(VRProxyTest, Send) {
  MockVRProxyListener listener;
  MockChannel *channel = new MockChannel();
  VRProxy proxy(&listener, channel);

  unsigned char value[] = { 0, 0, 0, 0x6, 0x8, 0, 0x10, 0x2, 0x18, 0x1 };
  std::string expected(value, value + 10);
  vr_hmi_api::ServiceMessage message;
  message.set_rpc(vr_hmi_api::ON_REGISTER);
  message.set_rpc_type(vr_hmi_api::NOTIFICATION);
  message.set_correlation_id(1);
  EXPECT_CALL(*channel, Send(expected)).Times(1).WillOnce(Return(true));
  EXPECT_TRUE(proxy.Send(message));
}

TEST_F(VRProxyTest, Receive) {
  MockVRProxyListener listener;
  MockChannel *channel = new MockChannel();
  VRProxy proxy(&listener, channel);

  unsigned char size[] = { 0, 0, 0, 0x6 };
  std::string in_size(size, size + 4);
  unsigned char data[] = { 0x8, 0, 0x10, 0x2, 0x18, 0x1 };
  std::string input(data, data + 6);
  vr_hmi_api::ServiceMessage message;
  message.set_rpc(vr_hmi_api::ON_REGISTER);
  message.set_rpc_type(vr_hmi_api::NOTIFICATION);
  message.set_correlation_id(1);
  EXPECT_CALL(*channel, Receive(4, _)).Times(1).WillOnce(
      DoAll(SetArgPointee<1>(in_size), Return(true)));
  EXPECT_CALL(*channel, Receive(6, _)).Times(1).WillOnce(
      DoAll(SetArgPointee<1>(input), Return(true)));
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  EXPECT_CALL(listener, OnReceived(ServiceMessageEq(message))).Times(1).WillOnce(
      Unlock(&mutex));
  pthread_mutex_lock(&mutex);
  proxy.Receive();
  pthread_mutex_lock(&mutex);
}

TEST_F(VRProxyTest, SizeToString) {
  MockVRProxyListener listener;
  MockChannel *channel = new MockChannel();
  VRProxy proxy(&listener, channel);

  unsigned char value[] = { 0, 0, 0, 123 };
  std::string input(value, value + 4);

  EXPECT_EQ(input, proxy.SizeToString(123));

  unsigned char value2[] = { 0, 0, 0x3, 0xBD };
  std::string input2(value2, value2 + 4);

  EXPECT_EQ(input2, proxy.SizeToString(957));
}

TEST_F(VRProxyTest, SizeFromString) {
  MockVRProxyListener listener;
  MockChannel *channel = new MockChannel();
  VRProxy proxy(&listener, channel);

  unsigned char value[] = { 0, 0, 0, 123 };
  std::string input(value, value + 4);

  EXPECT_EQ(uint32_t(123), proxy.SizeFromString(input));

  unsigned char value2[] = { 0, 0, 0x3, 0xBD };
  std::string input2(value2, value2 + 4);

  EXPECT_EQ(uint32_t(957), proxy.SizeFromString(input2));
}

}  // namespace vr_module
