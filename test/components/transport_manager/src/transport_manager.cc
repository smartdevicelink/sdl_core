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
#include "protocol_handler/protocol_handler_impl.h"
#include "transport_manager/info.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager_impl.h"

#include "transport_manager/raw_message_matcher.h"
#include "transport_manager/mock_device_adapter.h"
#include "transport_manager/mock_device.h"
#include "transport_manager/mock_transport_manager_listener.h"
#include "transport_manager/transport_manager_listener_impl.h"

using ::testing::_;
using ::testing::AtLeast;

using ::protocol_handler::RawMessage;

using ::transport_manager::ApplicationHandle;
using ::transport_manager::DeviceHandle;
using ::transport_manager::TransportManager;
using ::transport_manager::TransportManagerAttr;
using ::transport_manager::TransportManagerImpl;
using ::transport_manager::DeviceUID;
using ::transport_manager::DeviceInfo;

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
  static TransportManagerImpl *tm;
  static MockDeviceAdapter *mock_adapter;
  static MockTransportManagerListener *tm_listener;

  static void SetUpTestCase() {
    pthread_mutex_init(&test_mutex, NULL);
    pthread_cond_init(&test_cond, NULL);
    mock_adapter = new MockDeviceAdapter();
    mock_adapter->init();
    TransportManagerAttr cfg { 0 };

    tm = new TransportManagerImpl(cfg);
    protocol_handler::ProtocolHandlerImpl* protocol_handler = new protocol_handler::ProtocolHandlerImpl(tm); //FIXME this is a temporary workaround
    tm->set_protocol_handler(protocol_handler);

    tm_listener = new MockTransportManagerListener();
    tm->addEventListener(tm_listener);
    tm->addDeviceAdapter(mock_adapter);
    tm->init();
  }

  static void TearDownTestCase() {
    tm->removeEventListener(tm_listener);
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

TransportManagerImpl * TransportManagerTest::tm;

class MyTransportListener :
    public ::transport_manager::TransportManagerListenerImpl {
  TransportManagerTest * test;
 public:

  MyTransportListener(TransportManagerTest * test)
      : test(test),
        connection(0),
        device_handle(0) {
  }
  ConnectionUID connection;
  DeviceHandle device_handle;
  void OnConnectionEstablished(const DeviceInfo& device,
                               const ConnectionUID& connection_id) {
    connection = connection_id;

    pthread_mutex_lock(&test->test_mutex);
    pthread_cond_signal(&test->test_cond);
    pthread_mutex_unlock(&test->test_mutex);
  }

  void OnDeviceFound(const DeviceInfo& device_info) {
    device_handle = device_info.device_handle();
  }

  void OnScanDevicesFinished() {
    pthread_mutex_lock(&test->test_mutex);
    pthread_cond_signal(&test->test_cond);
    pthread_mutex_unlock(&test->test_mutex);
  }

  void OnTMMessageReceived(const RawMessageSptr message) {
    static int count = 0;
    if (++count == 25) {
      pthread_mutex_lock(&test->test_mutex);
      pthread_cond_signal(&test->test_cond);
      pthread_mutex_unlock(&test->test_mutex);
    }
  }
};

pthread_mutex_t TransportManagerTest::test_mutex;
pthread_cond_t TransportManagerTest::test_cond;

MockDeviceAdapter *TransportManagerTest::mock_adapter = nullptr;
MockTransportManagerListener *TransportManagerTest::tm_listener = nullptr;

TEST_F(TransportManagerTest, ScanDeviceFailed) {
  EXPECT_CALL(*tm_listener, OnDeviceFound(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_)).Times(1);
  EXPECT_CALL(*tm_listener, OnScanDevicesFinished()).Times(1).WillOnce(SignalTest(this));

  mock_adapter->get_device_scanner()->fail_further_search();
  tm->searchDevices();
  EXPECT_TRUE(waitCond(1));
  mock_adapter->get_device_scanner()->reset();
}

TEST_F(TransportManagerTest, ScanDeviceDone) {
  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnDeviceFound(_)).Times(1);
  EXPECT_CALL(*tm_listener, OnScanDevicesFinished()).Times(1).WillOnce(SignalTest(this));

  mock_adapter->get_device_scanner()->addDevice("TestDevice", "MA:CA:DR:ES:S");
  tm->searchDevices();
  EXPECT_TRUE(waitCond(1));
  mock_adapter->get_device_scanner()->reset();
}

TEST_F(TransportManagerTest, ScanManyDeviceDone) {
  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnDeviceFound(_)).Times(2);
  EXPECT_CALL(*tm_listener, OnScanDevicesFinished()).Times(1).WillOnce(SignalTest(this));
  mock_adapter->get_device_scanner()->addDevice("TestDevice1", "MA:CA:DR:ES:S1");
  mock_adapter->get_device_scanner()->addDevice("TestDevice2", "MA:CA:DR:ES:S2");
  tm->searchDevices();
  EXPECT_TRUE(waitCond(1));
  mock_adapter->get_device_scanner()->reset();
}

TEST_F(TransportManagerTest, ConnectDeviceDone) {
  const DeviceInfo kInfo(1, "MA:CA:DR:ES:S", "TestDeviceName");
  const ConnectionUID kConnection = 1;

  EXPECT_CALL(*tm_listener, OnDeviceFound(_)).Times(1);
  EXPECT_CALL(*tm_listener, OnScanDevicesFinished()).Times(1);
  EXPECT_CALL(*tm_listener, OnConnectionFailed(_, _)).Times(0);
  MyTransportListener *myListener = new MyTransportListener(this);
  mock_adapter->get_device_scanner()->addDevice(kInfo.name(), kInfo.mac_address());
  tm->addEventListener(myListener);
  tm->searchDevices();
  EXPECT_TRUE(waitCond(10));

  EXPECT_CALL(*tm_listener, OnConnectionEstablished(kInfo, kConnection)).Times(1);
  tm->connectDevice(myListener->device_handle);
  EXPECT_TRUE(waitCond(10));

  EXPECT_CALL(*tm_listener, OnConnectionClosed(myListener->connection)).Times(1).WillOnce(SignalTest(this));
  tm->disconnect(myListener->connection);

  EXPECT_TRUE(waitCond(10));
  tm->removeEventListener(myListener);
  delete myListener;
  mock_adapter->get_device_scanner()->reset();
}

TEST_F(TransportManagerTest, ConnectDeviceFailed) {
  const DeviceUID device_id = "TestDeviceName";
  const DeviceInfo info (1, "MA:CA:DR:ES:S", "TestDeviceName");
  EXPECT_CALL(*tm_listener, OnConnectionEstablished(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnConnectionFailed(_, _)).Times(1).WillOnce(SignalTest(this));

  tm->connectDevice(1);
  EXPECT_TRUE(waitCond(1));
  mock_adapter->get_device_scanner()->reset();
}

TEST_F(TransportManagerTest, DisconnectDeviceFailed) {
  const DeviceHandle kDevice = 2;
  EXPECT_CALL(*tm_listener, OnConnectionClosed(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnDeviceConnectionLost(kDevice, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnConnectionClosedFailure(_, _)).Times(1);
  EXPECT_CALL(*tm_listener, OnDisconnectFailed(kDevice, _)).Times(1).WillOnce(SignalTest(this));

  tm->disconnectDevice(kDevice);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, DisconnectDeviceDone) {
  const DeviceHandle kDevice = 3;
  const ApplicationHandle &kApplication = 1;
  EXPECT_CALL(*tm_listener, OnConnectionClosedFailure(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnDisconnectFailed(kDevice, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnConnectionClosed(_)).Times(1);
  EXPECT_CALL(*tm_listener, OnDeviceConnectionLost(kDevice, _)).Times(1).WillOnce(SignalTest(this));

  mock_adapter->connect("TestDevice", kApplication);
  tm->disconnectDevice(kDevice);
  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, SendReceive) {
  const DeviceUID device_id = "TestDeviceName";
  const DeviceInfo info(1, "MA:CA:DR:ES:S", "TestDeviceName");

  EXPECT_CALL(*tm_listener, OnDeviceFound(_)).Times(1);
  EXPECT_CALL(*tm_listener, OnScanDevicesFinished()).Times(1).WillOnce(SignalTest(this));

  MyTransportListener *myListener = new MyTransportListener(this);
  mock_adapter->get_device_scanner()->addDevice(info.name(), info.mac_address());
  tm->addEventListener(myListener);
  tm->searchDevices();
  EXPECT_TRUE(waitCond(10));

  EXPECT_CALL(*tm_listener, OnConnectionFailed(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnConnectionEstablished(_, _)).Times(1);
  tm->connectDevice(myListener->device_handle);
  EXPECT_TRUE(waitCond(10));

  EXPECT_CALL(*tm_listener, OnTMMessageSendFailed(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnTMMessageReceiveFailed(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnTMMessageSend()).Times(25);
  EXPECT_CALL(*tm_listener, OnTMMessageReceived(_)).Times(25);
  unsigned char *data = new unsigned char[2 * 1024 * 1024];
  for ( int i = 0; i < 25; ++i)
  {
    const RawMessageSptr kMessage = new RawMessage(myListener->connection, 1, data, 8 * 1024);
    tm->sendMessageToDevice(kMessage);
  }
  delete[] data;
  EXPECT_TRUE(waitCond(1));

  EXPECT_CALL(*tm_listener, OnConnectionClosed(_)).Times(1).WillOnce(SignalTest(this));
  tm->disconnect(myListener->connection);
  EXPECT_TRUE(waitCond(10));

  tm->removeEventListener(myListener);
  delete myListener;
  mock_adapter->get_device_scanner()->reset();
}

} // namespace transport_manager
} // namespace components
}  // namespace test

int main(int argc, char** argv) {
testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
}
