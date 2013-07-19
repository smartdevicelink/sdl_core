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

#include <transport_manager/transport_manager_impl.h>
#include <transport_manager/mock_device_adapter.h>
#include <transport_manager/mock_device_adapter_listener.h>
#include <transport_manager/mock_transport_manager_listener.h>
#include <protocol_handler/raw_message.h>
#include "transport_manager/device_handle_generator_impl.h"
#include "transport_manager/device_adapter_listener_impl.h"
#include "transport_manager/transport_manager_listener_impl.h"

using namespace transport_manager;
using namespace test::components::transport_manager;
using protocol_handler::RawMessage;

using testing::_;
using ::testing::Eq;
using ::testing::Pointee;
using ::testing::Property;
using ::testing::AtLeast;
using ::testing::MatcherInterface;
using ::testing::MatchResultListener;
using ::testing::Matcher;

class RawMessageSptrMatcher : public MatcherInterface<RawMessageSptr> {
 public:
  explicit RawMessageSptrMatcher(const unsigned char* data)
      : data_(data), data_size_(0) {}

  virtual bool MatchAndExplain(const RawMessageSptr ptr,
                               MatchResultListener* listener) const {
    unsigned char *d = ptr->data();
    unsigned int count = 0;
    data_size_ = ptr->data_size();
    for(int i = 0; i < ptr->data_size(); ++i){
      if(d[i] == data_[i])
        ++count;
    }
    return count == ptr->data_size();
  }

  virtual void DescribeTo(::std::ostream* os) const {
    *os << "data_ =  " ;
    for(int i = 0; i < data_size_; ++i){
      if(0 != data_[i])
        *os << data_[i];
    }
  }

  virtual void DescribeNegationTo(::std::ostream* os) const {
    *os << "data_ =  " ;
    for(int i = 0; i < data_size_; ++i){
      if (0 != data_[i])
      *os << data_[i];
    }
  }
 private:
  const unsigned char *data_;
  mutable unsigned int data_size_;
};

inline const Matcher<RawMessageSptr> RawMessageSptrEq(const unsigned char* data) {
  return MakeMatcher(new RawMessageSptrMatcher(data));
}

DeviceHandle hello;
ApplicationHandle hello_app;

//TEST(TransportManagerImplTest, connect)
//{
//  pthread_mutex_lock(&task_mutex);
//  TransportManagerImpl* tm = TransportManagerImpl::instance();
//  EXPECT_CALL(*tml, onConnectDone(_, 42)).Times(1);
//  tm->connectDevice(hello, hello_app, 42);
//
//  timespec elapsed;
//  clock_gettime(CLOCK_REALTIME, &elapsed);
//  elapsed.tv_sec += 1;
//  pthread_cond_timedwait(&task_complete, &task_mutex, &elapsed);
//  pthread_mutex_unlock(&task_mutex);
//}
//
//TEST(TransportManagerImplTest, sendReceive)
//{
//  pthread_mutex_lock(&task_mutex);
//  TransportManagerImpl* tm = TransportManagerImpl::instance();
//  const unsigned char data[100] = {99};
//  utils::SharedPtr<RawMessage> srm = new RawMessage(42, 1, const_cast<unsigned char *>(data), 100);
//
//  tm->sendMessageToDevice(srm);
//
//  EXPECT_CALL(*tml, onDataSendDone(_, _, _)).Times(AtLeast(1));
//  EXPECT_CALL(*tml, onDataSendFailed(_, _, _)).Times(AtLeast(0));
//  EXPECT_CALL(*tml, onDataReceiveDone(_, _, RawMessageSptrEq(data))).Times(AtLeast(1));
//  EXPECT_CALL(*tml, onDataReceiveFailed(_, _, _)).Times(AtLeast(0));
//
//  timespec elapsed;
//  clock_gettime(CLOCK_REALTIME, &elapsed);
//  elapsed.tv_sec += 1;
//  pthread_cond_timedwait(&task_complete, &task_mutex, &elapsed);
//  pthread_mutex_unlock(&task_mutex);
//}
//
//TEST(TransportManagerImplTest, disconnectDevice)
//{
//  pthread_mutex_lock(&task_mutex);
//  TransportManagerImpl* tm = TransportManagerImpl::instance();
//  tm->disconnectDevice(0);
//  EXPECT_CALL(*tml, onDisconnectDeviceDone(_, _)).Times(1);
//
//  timespec elapsed;
//  clock_gettime(CLOCK_REALTIME, &elapsed);
//  elapsed.tv_sec += 1;
//  pthread_cond_timedwait(&task_complete, &task_mutex, &elapsed);
//  pthread_mutex_unlock(&task_mutex);
//}

ACTION_P2(WaitTest, mutex, cond)
{
  pthread_mutex_lock(mutex);
  pthread_cond_signal(cond);
  pthread_mutex_unlock(mutex);
}

class TransportManagerTest : public testing::Test {
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
    mock_da->init(new DeviceHandleGeneratorImpl(), NULL);
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
  TransportManagerImpl* tm = TransportManagerImpl::instance();

  EXPECT_CALL(*tml, onDeviceFound(_, _)).Times(0);
  EXPECT_CALL(*tml, onSearchDeviceFailed(_, _)).Times(1)
      .WillOnce(WaitTest(&test_mutex, &test_cond));

  tm->searchDevices();

  EXPECT_TRUE(waitCond(1));
}

TEST_F(TransportManagerTest, SearchDeviceDone)
{
  TransportManagerImpl* tm = TransportManagerImpl::instance();
  mock_da->addDevice("TestDevice");

  EXPECT_CALL(*tml, onSearchDeviceFailed(_, _)).Times(0);
  EXPECT_CALL(*tml, onDeviceFound(_, _)).Times(1)
      .WillOnce(WaitTest(&test_mutex, &test_cond));

  tm->searchDevices();

  EXPECT_TRUE(waitCond(1));
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
