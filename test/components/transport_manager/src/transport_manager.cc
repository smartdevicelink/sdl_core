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
#include <pthread.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "protocol_handler/raw_message.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager_impl.h"
#include "transport_manager/device_handle_generator_impl.h"

#include "transport_manager/raw_message_matcher.h"
#include "transport_manager/mock_device_adapter.h"
#include "transport_manager/mock_device.h"
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

ACTION_P(SignalTest, test){
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
  static MockDeviceAdapter *mock_adapter;
  static MockTransportManagerListener *tm_listener;

  static void SetUpTestCase() {
    pthread_mutex_init(&test_mutex, NULL);
    pthread_cond_init(&test_cond, NULL);
    tm_listener = new MockTransportManagerListener();
    mock_adapter = new MockDeviceAdapter();
    mock_adapter->init(NULL);
    TransportManager* tm = TransportManagerImpl::instance();
    tm->addEventListener(tm_listener);
    tm->addDeviceAdapter(mock_adapter);
  }

  static void TearDownTestCase() {
    delete tm_listener;
    pthread_cond_destroy(&test_cond);
    pthread_mutex_destroy(&test_mutex);
  }

  virtual void SetUp() {
    one_thread = false;
    thread_id = pthread_self();
    mock_adapter->reset();
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

MockDeviceAdapter *TransportManagerTest::mock_adapter = nullptr;
MockTransportManagerListener *TransportManagerTest::tm_listener = nullptr;

TEST_F(TransportManagerTest, Instance) {
  TransportManagerImpl* prev_impl = TransportManagerImpl::instance();
  ASSERT_EQ(prev_impl, TransportManagerImpl::instance());
}

TEST_F(TransportManagerTest, SearchDeviceFailed) {
  EXPECT_CALL(*tm_listener, OnDeviceFound(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnScanDevicesFinished()).Times(1);
  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_)).Times(1).WillOnce(
      SignalTest(this));

  mock_adapter->get_device_scanner()->set_is_search_failed();
  TransportManagerImpl::instance()->searchDevices();
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, SearchDeviceDone) {
  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnDeviceFound(_, _)).Times(1);
  EXPECT_CALL(*tm_listener,OnScanDevicesFinished()).Times(1).WillOnce(
      SignalTest(this));

  mock_adapter->get_device_scanner()->addDevice("TestDevice");
  TransportManagerImpl::instance()->searchDevices();
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, SearchManyDeviceDone) {
  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnDeviceFound(_, _)).Times(2);
  EXPECT_CALL(*tm_listener,OnScanDevicesFinished()).Times(1).WillOnce(
      SignalTest(this));

  mock_adapter->get_device_scanner()->addDevice("TestDevice1");
  mock_adapter->get_device_scanner()->addDevice("TestDevice2");
  TransportManagerImpl::instance()->searchDevices();
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, ConnectDeviceDone) {
  const DeviceHandle kDevice = "TestDevice";
  const ApplicationHandle kApplication = 1;
  EXPECT_CALL(*tm_listener, OnConnectionFailed(kDevice, kApplication, _)).Times(
      0);
  EXPECT_CALL(*tm_listener, OnConnectionEstablished(kDevice, kApplication, _)).
      Times(1).WillOnce(SignalTest(this));

  mock_adapter->get_device_scanner()->addDevice("TestDevice");
  TransportManagerImpl::instance()->connectDevice(kDevice, kApplication);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, ConnectDeviceFailed) {
  const DeviceHandle kDevice = "NoDevice";
  const ApplicationHandle &kApplication = 0;
  EXPECT_CALL(*tm_listener, OnConnectionEstablished(kDevice, kApplication, _)).
      Times(0);
  EXPECT_CALL(*tm_listener, OnConnectionFailed(kDevice, kApplication, _)).Times(
      1).WillOnce(SignalTest(this));

  TransportManagerImpl::instance()->connectDevice(kDevice, kApplication);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, DisconnectDeviceFailed) {
  const DeviceHandle kDevice = "0";
  EXPECT_CALL(*tm_listener, OnConnectionClosed(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnDeviceConnectionLost(kDevice, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnConnectionClosedFailure(_, _)).Times(1);
  EXPECT_CALL(*tm_listener, OnDisconnectFailed(kDevice, _)).Times(1).WillOnce(
      SignalTest(this));

  TransportManagerImpl::instance()->disconnectDevice(kDevice);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, DisconnectDeviceDone) {
  const DeviceHandle kDevice = "TestDevice";
  const ApplicationHandle &kApplication = 1;
  EXPECT_CALL(*tm_listener, OnConnectionClosedFailure(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnDisconnectFailed(kDevice, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnConnectionClosed(_)).Times(1);
  EXPECT_CALL(*tm_listener, OnDeviceConnectionLost(kDevice, _)).Times(1)
      .WillOnce(SignalTest(this));

  mock_adapter->connect("TestDevice", kApplication);
  TransportManagerImpl::instance()->disconnectDevice(kDevice);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, SendReceive) {
  const ConnectionUID kSession = 42;
  const int kVersionProtocol = 1;
  const unsigned int kSize = 100;
  unsigned char data[kSize] = { 99 };
  const RawMessageSptr kMessage = new RawMessage(kSession, kVersionProtocol,
                                                 data, kSize);

  EXPECT_CALL(*tm_listener, OnTMMessageSendFailed(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnTMMessageReceiveFailed(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnTMMessageSend()).Times(1);
  EXPECT_CALL(*tm_listener, OnTMMessageReceived(RawMessageEq(kMessage))).Times(
      1).WillOnce(SignalTest(this));

  TransportManagerImpl::instance()->sendMessageToDevice(kMessage);
  EXPECT_TRUE(waitCond(1));
}

}  // namespace transport_manager
}  // namespace components
}  // namespace test

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
