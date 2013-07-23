/*
 * \file transport_manager.cc
 * \brief
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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "protocol_handler/raw_message.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager_impl.h"
#include "transport_manager/device_handle_generator_impl.h"

//#include "transport_manager/matchers.h"
#include "transport_manager/mock_device_adapter.h"
#include "transport_manager/mock_transport_manager_listener.h"

using ::testing::_;
using ::protocol_handler::RawMessage;

namespace test {
namespace components {
namespace transport_manager {

ACTION_P2(WaitTest, mutex, cond)
{
  pthread_mutex_lock(mutex);
  pthread_cond_signal(cond);
  pthread_mutex_unlock(mutex);
}

class TransportManagerTest : public ::testing::Test {
 protected:
  static pthread_mutex_t test_mutex;
  static pthread_cond_t test_cond;
  static MockDeviceAdapter *mock_da;
  static MockTransportManagerListener *tml;

  static void SetUpTestCase() {
    pthread_mutex_init(&test_mutex, NULL);
    pthread_cond_init(&test_cond, NULL);
    tml = new MockTransportManagerListener();
    mock_da = new MockDeviceAdapter();
    mock_da->init(NULL);
    TransportManager* tm = TransportManagerImpl::instance();
    tm->addEventListener(tml);
    tm->addDeviceAdapter(mock_da);
  }

  static void TearDownTestCase() {
    pthread_cond_destroy(&test_cond);
    pthread_mutex_destroy(&test_mutex);
  }

  virtual void SetUp() {
    pthread_mutex_lock(&test_mutex);
  }

  virtual void TearDown() {
    pthread_mutex_unlock(&test_mutex);
  }

  bool waitCond(int seconds) {
    timespec elapsed;
    clock_gettime(CLOCK_REALTIME, &elapsed);
    elapsed.tv_sec += seconds;
    return pthread_cond_timedwait(&test_cond, &test_mutex, &elapsed) == 0;
  }
};

pthread_mutex_t TransportManagerTest::test_mutex;
pthread_cond_t TransportManagerTest::test_cond;
MockDeviceAdapter *TransportManagerTest::mock_da = 0;
MockTransportManagerListener *TransportManagerTest::tml = 0;

TEST_F(TransportManagerTest, instance)
{
  TransportManagerImpl* prev_impl = TransportManagerImpl::instance();
  ASSERT_EQ(prev_impl, TransportManagerImpl::instance());
}

TEST_F(TransportManagerTest, SearchDeviceFailed)
{
  EXPECT_CALL(*tml, onDeviceFound(_, _)).Times(0);
  EXPECT_CALL(*tml, onSearchDeviceDone()).Times(0);
  EXPECT_CALL(*tml, onSearchDeviceFailed(_, _)).Times(1)
      .WillOnce(WaitTest(&test_mutex, &test_cond));

  TransportManagerImpl::instance()->searchDevices();
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, SearchDeviceDone)
{
  EXPECT_CALL(*tml, onSearchDeviceFailed(_, _)).Times(0);
  EXPECT_CALL(*tml, onDeviceFound(_, _)).Times(1);
  EXPECT_CALL(*tml, onSearchDeviceDone()).Times(1)
      .WillOnce(WaitTest(&test_mutex, &test_cond));

  mock_da->addDevice("TestDevice");
  TransportManagerImpl::instance()->searchDevices();
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, ConnectDeviceDone)
{
  const ConnectionId kSession = 42;
  EXPECT_CALL(*tml, onConnectFailed(_, kSession, _)).Times(0);
  EXPECT_CALL(*tml, onConnectDone(_, kSession)).Times(1)
      .WillOnce(WaitTest(&test_mutex, &test_cond));

  const DeviceHandle kDevice = "1";
  const ApplicationHandle kApplication = 1;
  TransportManagerImpl::instance()->connectDevice(kDevice, kApplication);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, ConnectDeviceFailed)
{
  const ConnectionId kSession = 333;
  EXPECT_CALL(*tml, onConnectDone(_, kSession)).Times(0);
  EXPECT_CALL(*tml, onConnectFailed(_, kSession, _)).Times(1)
      .WillOnce(WaitTest(&test_mutex, &test_cond));

  const DeviceHandle kDevice = "333";
  const ApplicationHandle kApplication = 333;
  TransportManagerImpl::instance()->connectDevice(kDevice, kApplication);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, DisconnectDeviceFailed)
{
  const DeviceHandle kDevice = "0";
  EXPECT_CALL(*tml, onDisconnectDone(_, _)).Times(0);
  EXPECT_CALL(*tml, onDisconnectDeviceDone(_, kDevice)).Times(0);
  EXPECT_CALL(*tml, onDisconnectFailed(_, _, _)).Times(1);
  EXPECT_CALL(*tml, onDisconnectDeviceFailed(_, kDevice, _)).Times(1)
    .WillOnce(WaitTest(&test_mutex, &test_cond));

  TransportManagerImpl::instance()->disconnectDevice(kDevice);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, DisconnectDeviceDone)
{
  const DeviceHandle kDevice = "1";
  EXPECT_CALL(*tml, onDisconnectFailed(_, _, _)).Times(0);
  EXPECT_CALL(*tml, onDisconnectDeviceFailed(_, kDevice, _)).Times(0);
  EXPECT_CALL(*tml, onDisconnectDone(_, _)).Times(1);
  EXPECT_CALL(*tml, onDisconnectDeviceDone(_, kDevice)).Times(1)
    .WillOnce(WaitTest(&test_mutex, &test_cond));

  TransportManagerImpl::instance()->disconnectDevice(kDevice);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, SendReceive)
{
  const ConnectionId kSession = 42;
  const int kVersionProtocol = 1;
  const unsigned int kSize = 100;
  unsigned char data[kSize] = {99};
  RawMessageSptr message = new RawMessage(kSession, kVersionProtocol, data, kSize);

  EXPECT_CALL(*tml, onDataSendFailed(_, _, _)).Times(0);
  EXPECT_CALL(*tml, onDataReceiveFailed(_, _, _)).Times(0);
  EXPECT_CALL(*tml, onDataSendDone(_, kSession, message)).Times(1);
  EXPECT_CALL(*tml, onDataReceiveDone(_, kSession, message)).Times(1)
    .WillOnce(WaitTest(&test_mutex, &test_cond));

  TransportManagerImpl::instance()->sendMessageToDevice(message);
  EXPECT_TRUE(waitCond(1));
}

}  // namespace transport_manager
}  // namespace components
}  // namespace test

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
