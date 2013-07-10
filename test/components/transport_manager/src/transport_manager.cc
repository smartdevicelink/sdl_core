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


TEST(TransportManagerImpl, instance)
{
  TransportManagerImpl* prev_impl = TransportManagerImpl::instance();
  ASSERT_EQ(prev_impl, TransportManagerImpl::instance());
}

DeviceHandle hello;
ApplicationHandle hello_app;
volatile bool flag = false;

class MyListener : public TransportManagerListenerImpl {
  void onSearchDeviceDone(const DeviceHandle device,
                          const ApplicationList app_list) {
    hello = device;
    hello_app = *app_list.begin();
    flag = true;
  }
  void onConnectDone(const DeviceAdapter* device_adapter,
                     const transport_manager::SessionID session_id) {
    flag = true;
  }
  void onDataReceiveDone(const DeviceAdapter* device_adapter,
                         const transport_manager::SessionID session_id,
                         const RawMessageSptr data_container) {
    flag = true;

  }
};

TEST(TransportManagerImpl, searchDevice)
{
  TransportManagerImpl* tm = TransportManagerImpl::instance();

  MockDeviceAdapter *mock_da = new MockDeviceAdapter();
  tm->addDeviceAdapter(mock_da);

  MockTransportManagerListener *tml = new MockTransportManagerListener();
  tm->addEventListener(tml);
  tm->addEventListener(new MyListener());
  tm->addAdapterListener(mock_da, new DeviceAdapterListenerImpl(tm));

  //MockDeviceAdapterListener *mdal = new MockDeviceAdapterListener();

  //impl->addAdapterListener(mock_da, mdal);

  mock_da->init(new DeviceHandleGeneratorImpl(),
      NULL);

  const unsigned char data[100] = {99};
  utils::SharedPtr<RawMessage> srm = new RawMessage(42, 1, const_cast<unsigned char *>(data), 100);

  EXPECT_CALL(*tml, onSearchDeviceDone(_, _)).Times(AtLeast(1));
  EXPECT_CALL(*tml, onSearchDeviceFailed(_, _)).Times(AtLeast(0));
  EXPECT_CALL(*tml, onConnectDone(_, 42)).Times(1);
  EXPECT_CALL(*tml, onDataSendFailed(_, _, _)).Times(AtLeast(0));
  EXPECT_CALL(*tml, onDataReceiveDone(_, _, RawMessageSptrEq(data))).Times(AtLeast(1));
  EXPECT_CALL(*tml, onDataReceiveFailed(_, _, _)).Times(AtLeast(0));

  tm->searchDevices();

  while(!flag) {}
  flag = false;

  tm->connectDevice(hello, hello_app, 42);
  while(!flag) {}
  flag = false;


  tm->sendMessageToDevice(srm);

  while(!flag) {}
  flag = false;

}
