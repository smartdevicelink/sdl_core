/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "protocol/common.h"
#include "transport_manager/info.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager_impl.h"

#include "include/raw_message_matcher.h"
#include "include/mock_transport_adapter.h"
#include "include/mock_device.h"
#include "include/mock_transport_manager_listener.h"
#include "transport_manager/transport_manager_listener_empty.h"

//for instance test
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_adapter/transport_adapter.h"
#include "transport_manager/transport_manager_default.h"
#include "connection_handler/connection_handler.h"

using ::testing::_;
using ::testing::AtLeast;

using ::transport_manager::ApplicationHandle;
using ::transport_manager::DeviceHandle;
using ::transport_manager::TransportManager;
using ::transport_manager::TransportManagerImpl;
using ::transport_manager::DeviceUID;
using ::transport_manager::DeviceInfo;

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
  int number;

 protected:
  static TransportManagerImpl *tm;
  static MockTransportAdapter *mock_adapter;
  static MockTransportManagerListener *tm_listener;

  static void SetUpTestCase() {
    pthread_mutex_init(&test_mutex, NULL);
    pthread_cond_init(&test_cond, NULL);
    mock_adapter = new MockTransportAdapter();
    mock_adapter->Init();
    //TransportManagerAttr cfg{0};
    tm = new TransportManagerImpl();

    tm_listener = new MockTransportManagerListener();
    tm->AddEventListener(tm_listener);
    tm->AddTransportAdapter(mock_adapter);
    tm->Init();
  }

  static void TearDownTestCase() {
    tm->Stop();
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

  virtual void TearDown() { pthread_mutex_unlock(&test_mutex); }

  bool waitCond(int seconds) {
    if (one_thread) return true;
    timespec elapsed;
    clock_gettime(CLOCK_REALTIME, &elapsed);
    elapsed.tv_sec += seconds;
    return pthread_cond_timedwait(&test_cond, &test_mutex, &elapsed) !=
           ETIMEDOUT;
  }
};

TransportManagerImpl *TransportManagerTest::tm;

class MyTransportListener
    : public ::transport_manager::TransportManagerListenerEmpty {
     public:
  explicit MyTransportListener(TransportManagerTest *test)
      : TransportManagerListenerEmpty(),
        connection(0),
        device_handle(0),
        test(test) {}
  ConnectionUID connection;
  DeviceHandle device_handle;
  void OnConnectionEstablished(const DeviceInfo &device,
                               const ConnectionUID &connection_id) {
    connection = connection_id;

    pthread_mutex_lock(&test->test_mutex);
    pthread_cond_signal(&test->test_cond);
    pthread_mutex_unlock(&test->test_mutex);
  }

  void OnDeviceFound(const DeviceInfo &device_info) {
    device_handle = device_info.device_handle();
  }

  void OnScanDevicesFinished() {
    pthread_mutex_lock(&test->test_mutex);
    pthread_cond_signal(&test->test_cond);
    pthread_mutex_unlock(&test->test_mutex);
  }

  void OnTMMessageReceived(const RawMessagePtr message) {
    static int count = 0;
    if (++count == 100) {
      pthread_mutex_lock(&test->test_mutex);
      pthread_cond_signal(&test->test_cond);
      pthread_mutex_unlock(&test->test_mutex);
    }
  }

  void OnTMMessageSend(const RawMessagePtr message) {
  }

 private:
  TransportManagerTest *test;
};

pthread_mutex_t TransportManagerTest::test_mutex;
pthread_cond_t TransportManagerTest::test_cond;

MockTransportAdapter *TransportManagerTest::mock_adapter = NULL;
MockTransportManagerListener *TransportManagerTest::tm_listener = NULL;



TEST_F(TransportManagerTest, ScanDeviceFailed) {

  //assert
  EXPECT_CALL(*tm_listener, OnDeviceFound(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_)).WillOnce(
      SignalTest(this));

  //act
  mock_adapter->get_device_scanner()->fail_further_search();
  tm->SearchDevices();

  //assert
  EXPECT_TRUE(waitCond(1));

  //act
  mock_adapter->get_device_scanner()->reset();
}

TEST_F(TransportManagerTest, ScanDeviceNoFound) {

  //assert
  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnDeviceFound(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnScanDevicesFinished()).WillOnce(SignalTest(this));

  EXPECT_CALL(*tm_listener, OnDeviceListUpdated(_));

  //act
  tm->SearchDevices();

  //assert
  EXPECT_TRUE(waitCond(1));

  //act
  mock_adapter->get_device_scanner()->reset();
}

TEST_F(TransportManagerTest, ScanDeviceDone) {

  //assert
  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnDeviceFound(_));
  EXPECT_CALL(*tm_listener, OnScanDevicesFinished()).WillOnce(SignalTest(this));

  //added to fixed warnings
  EXPECT_CALL(*tm_listener, OnDeviceAdded(_)).Times(3);
  EXPECT_CALL(*tm_listener, OnDeviceListUpdated(_));
  EXPECT_CALL(*tm_listener, OnDeviceRemoved(_)).Times(3);

  //act
  mock_adapter->get_device_scanner()->AddDevice("TestDevice", "MA:CA:DR:ES:S");
  tm->SearchDevices();

  //assert
  EXPECT_TRUE(waitCond(1));

  //act
  mock_adapter->get_device_scanner()->reset();
}

TEST_F(TransportManagerTest, ScanManyDeviceDone) {


  //assert
  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_)).Times(0);
  EXPECT_CALL(*tm_listener, OnDeviceFound(_)).Times(2);
  EXPECT_CALL(*tm_listener, OnScanDevicesFinished()).WillOnce(SignalTest(this));

  //assert
  EXPECT_CALL(*tm_listener, OnDeviceListUpdated(_));
  //act
  mock_adapter->get_device_scanner()->AddDevice("TestDevice1",
                                                "MA:CA:DR:ES:S1");
  mock_adapter->get_device_scanner()->AddDevice("TestDevice2",
                                                "MA:CA:DR:ES:S2");
  tm->SearchDevices();

  //assert
  EXPECT_TRUE(waitCond(1));

  //act
  mock_adapter->get_device_scanner()->reset();
}


TEST_F(TransportManagerTest, ConnectAddDeviceCannotFailConnection) {

  //arrange
  const DeviceInfo kInfo(1, "MA:CA:DR:ES:S", "TestDeviceName", "BTMAC");
  const ConnectionUID kConnection = 1;

  //assert
  EXPECT_CALL(*tm_listener, OnDeviceFound(_));
  EXPECT_CALL(*tm_listener, OnScanDevicesFinished());

  EXPECT_CALL(*tm_listener, OnDeviceAdded(_));
  EXPECT_CALL(*tm_listener, OnDeviceListUpdated(_));

  //act
  MyTransportListener *myListener = new MyTransportListener(this);
  mock_adapter->get_device_scanner()->AddDevice(kInfo.name(),
                                                kInfo.mac_address());
  tm->AddEventListener(myListener);
  tm->SearchDevices();

  //assert
  EXPECT_TRUE(waitCond(10));

  EXPECT_CALL(*tm_listener, OnConnectionFailed(_, _)).Times(0);
  EXPECT_CALL(*tm_listener, OnConnectionEstablished(kInfo, kConnection));

  //act
  tm->ConnectDevice(kInfo.device_handle());

  //assert
  EXPECT_TRUE(waitCond(10));

  //act
  mock_adapter->get_device_scanner()->reset();
}


//TODO{ALeshin}: APPLINK-10846
//TEST_F(TransportManagerTest, ConnectDeviceSendReciveMessage) {

//  //arrange
//  const ConnectionUID kConnection = 1;
//  const int kTimes = 99;  // Times of send message //if kTimes>99 OnTMMessageSend will fail
//  const unsigned int kVersionProtocol = 1;


//  //assert
//  EXPECT_CALL(*tm_listener, OnTMMessageSendFailed(_, _)).Times(0);
//  EXPECT_CALL(*tm_listener, OnTMMessageReceiveFailed(_, _)).Times(0);
//  EXPECT_CALL(*tm_listener, OnConnectionClosed(kConnection)).Times(0);

//  EXPECT_CALL(*tm_listener, OnTMMessageSend(_)).Times(kTimes);
//  EXPECT_CALL(*tm_listener, OnTMMessageReceived(_)).Times(kTimes);

//  //act
//  const unsigned int kSize = 12;
//  unsigned char data[kSize] = {0x20, 0x07, 0x01, 0x00, 0x00, 0x00,
//                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//  for (int i = 0; i < kTimes; ++i) {
//   const RawMessagePtr kMessage =
//    new RawMessage(kConnection, kVersionProtocol, data, kSize);
//    tm->SendMessageToDevice(kMessage);
//    usleep(1000);
//  }

//  //assert
//  EXPECT_TRUE(waitCond(10));

//  //act
//  mock_adapter->get_device_scanner()->reset();
//}

//TODO{ALeshin}: APPLINK-10846
//TEST_F(TransportManagerTest, ConnectAddDeviceCannotFailClose) {

//  //arrange
//  const DeviceInfo kInfo(1, "MA:CA:DR:ES:S", "TestDeviceName", "BTMAC");
//  const ConnectionUID kConnection = 1;

//  //assert
//  EXPECT_CALL(*tm_listener, OnDeviceFound(_));
//  EXPECT_CALL(*tm_listener, OnScanDevicesFinished());
//  EXPECT_CALL(*tm_listener, OnDeviceListUpdated(_));


//  //act
//  MyTransportListener *myListener = new MyTransportListener(this);
//  mock_adapter->get_device_scanner()->AddDevice(kInfo.name(),
//                                                      kInfo.mac_address());
//  tm->AddEventListener(myListener);
//  tm->SearchDevices();

//  //assert
//  EXPECT_TRUE(waitCond(10));

//  EXPECT_CALL(*tm_listener, OnConnectionClosedFailure(_, _)).Times(0);
//  EXPECT_CALL(*tm_listener, OnDisconnectFailed(kInfo.device_handle(), _))
//            .Times(0);
//  EXPECT_CALL(*tm_listener, OnConnectionClosed(kConnection)).WillOnce(SignalTest(this));

//  //act
//  tm->DisconnectDevice(kInfo.device_handle());

//  //assert
//  EXPECT_TRUE(waitCond(10));

//  //act
//  tm->Stop();
//  delete myListener;
//  mock_adapter->get_device_scanner()->reset();
//}


}  // namespace transport_manager
}  // namespace components
}  // namespace test


namespace test{
namespace test_transport_manager_instance {
TEST(testTransportManager, CreateOnlyInstance)
{
  transport_manager::TransportManager *Instance = transport_manager::TransportManagerDefault::instance();
  ASSERT_EQ(Instance, transport_manager::TransportManagerDefault::instance());
  delete Instance;
}
}}
