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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "protocol_handler/raw_message.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager_impl.h"
#include "transport_manager/device_handle_generator_impl.h"

#include "transport_manager/raw_message_matcher.h"
#include "transport_manager/fake_device_adapter.h"
#include "transport_manager/mock_transport_manager_listener.h"

using ::testing::_;

using ::protocol_handler::RawMessage;

using ::transport_manager::ApplicationHandle;
using ::transport_manager::DeviceHandle;
using ::transport_manager::TransportManager;
using ::transport_manager::TransportManagerImpl;

namespace test {
namespace components {
namespace transport_manager {

ACTION_P(SignalTest, test) {
  if (test->thread_id != pthread_self()) {
    pthread_mutex_lock(&test->test_mutex);
    pthread_cond_signal(&test->test_cond);
    pthread_mutex_unlock(&test->test_mutex);
  } else {
    test->one_thread = true;
  }
}

class TransportManagerTest : public ::testing::Test {
 public:
  volatile bool one_thread;
  pthread_t thread_id;
  static pthread_mutex_t test_mutex;
  static pthread_cond_t test_cond;
 protected:
  static FakeDeviceAdapter *fake_adapter;
  static MockTransportManagerListener *tm_listener;

  static void SetUpTestCase() {
    pthread_mutex_init(&test_mutex, NULL);
    pthread_cond_init(&test_cond, NULL);
    tm_listener = new MockTransportManagerListener();
    fake_adapter = new FakeDeviceAdapter();
    fake_adapter->init(NULL);
    TransportManager* tm = TransportManagerImpl::instance();
    tm->addEventListener(tm_listener);
    tm->addDeviceAdapter(fake_adapter);
  }

  static void TearDownTestCase() {
    delete tm_listener;
    pthread_cond_destroy(&test_cond);
    pthread_mutex_destroy(&test_mutex);
  }

  virtual void SetUp() {
    one_thread = false;
    thread_id = pthread_self();
    pthread_mutex_lock(&test_mutex);
  }

  virtual void TearDown() {
    pthread_mutex_unlock(&test_mutex);
  }

  bool waitCond(int seconds) {
    if (one_thread)
      return true;
    timespec elapsed;
    clock_gettime(CLOCK_REALTIME, &elapsed);
    elapsed.tv_sec += seconds;
    return pthread_cond_timedwait(&test_cond, &test_mutex, &elapsed) == 0;
  }
};

pthread_mutex_t TransportManagerTest::test_mutex;
pthread_cond_t TransportManagerTest::test_cond;
FakeDeviceAdapter *TransportManagerTest::fake_adapter = 0;
MockTransportManagerListener *TransportManagerTest::tm_listener = 0;

TEST_F(TransportManagerTest, Instance)
{
  TransportManagerImpl* prev_impl = TransportManagerImpl::instance();
  ASSERT_EQ(prev_impl, TransportManagerImpl::instance());
}

TEST_F(TransportManagerTest, SearchDeviceFailed)
{
  EXPECT_CALL(*tm_listener, onDeviceFound(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, onSearchDeviceDone()).Times(0);
  EXPECT_CALL(*tm_listener, onSearchDeviceFailed(_, _)).Times(1)
      .WillOnce(SignalTest(this));

  fake_adapter->clearDevices();
  TransportManagerImpl::instance()->searchDevices();
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, SearchDeviceDone)
{
  EXPECT_CALL(*tm_listener, onSearchDeviceFailed(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, onDeviceFound(_, _)).Times(1);
  EXPECT_CALL(*tm_listener,onSearchDeviceDone()).Times(1)
      .WillOnce(SignalTest(this));

  fake_adapter->addDevice("TestDevice");
  TransportManagerImpl::instance()->searchDevices();
  EXPECT_TRUE(waitCond(1));
  fake_adapter->clearDevices();
}

TEST_F(TransportManagerTest, ConnectDeviceDone)
{
  const DeviceHandle kDevice = "TestDevice";
  const ApplicationHandle kApplication = 1;
  EXPECT_CALL(*tm_listener, onConnectFailed(_, kDevice, kApplication, _)).Times(0);
  EXPECT_CALL(*tm_listener, onConnectDone(_, kDevice, kApplication, _)).Times(1)
      .WillOnce(SignalTest(this));

  fake_adapter->addDevice("TestDevice");
  TransportManagerImpl::instance()->connectDevice(kDevice, kApplication);
  EXPECT_TRUE(waitCond(1));
  fake_adapter->clearDevices();
}

TEST_F(TransportManagerTest, ConnectDeviceFailed)
{
  const DeviceHandle kDevice = "NoDevice";
  const ApplicationHandle kApplication = 0;
  EXPECT_CALL(*tm_listener, onConnectDone(_, kDevice, kApplication, _)).Times(0);
  EXPECT_CALL(*tm_listener, onConnectFailed(_, kDevice, kApplication, _)).Times(0)
      .WillOnce(SignalTest(this));

  fake_adapter->clearDevices();
  TransportManagerImpl::instance()->connectDevice(kDevice, kApplication);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, DISABLED_DisconnectDeviceFailed)
{
  const DeviceHandle kDevice = "0";
  EXPECT_CALL(*tm_listener, onDisconnectDone(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, onDisconnectDeviceDone(_, kDevice)).Times(0);
  EXPECT_CALL(*tm_listener, onDisconnectFailed(_, _, _)).Times(1);
  EXPECT_CALL(*tm_listener, onDisconnectDeviceFailed(_, kDevice, _)).Times(1)
    .WillOnce(SignalTest(this));

  TransportManagerImpl::instance()->disconnectDevice(kDevice);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, DISABLED_DisconnectDeviceDone)
{
  const DeviceHandle kDevice = "TestDevice";
  const ApplicationHandle kApplication = 1;
  EXPECT_CALL(*tm_listener, onDisconnectFailed(_, _, _)).Times(0);
  EXPECT_CALL(*tm_listener, onDisconnectDeviceFailed(_, kDevice, _)).Times(0);
  EXPECT_CALL(*tm_listener, onDisconnectDone(_, _)).Times(1);
  EXPECT_CALL(*tm_listener, onDisconnectDeviceDone(_, kDevice)).Times(1)
        .WillOnce(SignalTest(this));

  fake_adapter->addConnection("TestDevice", kApplication);
  TransportManagerImpl::instance()->disconnectDevice(kDevice);
  EXPECT_TRUE(waitCond(1));
  fake_adapter->clearConnection();
}

TEST_F(TransportManagerTest, DISABLED_SendReceive)
{
  const ConnectionId kSession = 42;
  const int kVersionProtocol = 1;
  const unsigned int kSize = 100;
  unsigned char data[kSize] = {99};
  const RawMessageSptr kMessage = new RawMessage(kSession, kVersionProtocol, data, kSize);

  EXPECT_CALL(*tm_listener, onDataSendFailed(_, _, _)).Times(0);
  EXPECT_CALL(*tm_listener, onDataReceiveFailed(_, _, _)).Times(0);
  EXPECT_CALL(*tm_listener, onDataSendDone(_, kSession, RawMessageEq(kMessage))).Times(1);
  EXPECT_CALL(*tm_listener, onDataReceiveDone(_, kSession, RawMessageEq(kMessage))).Times(1)
  .WillOnce(SignalTest(this));

  TransportManagerImpl::instance()->sendMessageToDevice(kMessage);
  EXPECT_TRUE(waitCond(1));
}

// TODO (KKolodiy): tests for disconnect pair(device,app) by connection id (fail, done)

}  // namespace transport_manager
}  // namespace components
}  // namespace test

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
