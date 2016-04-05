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

#include "gtest/gtest.h"
#include "include/transport_adapter_mock.h"
#include "transport_manager/transport_adapter/transport_adapter_listener_impl.h"
#include "transport_manager/mock_transport_manager.h"

namespace test {
namespace components {
namespace transport_manager_test {

using ::testing::Return;
using namespace ::transport_manager;

class TransportAdapterListenerTest : public ::testing::Test {
 public:
  TransportAdapterListenerTest()
      : app_handle(1),
        dev_id("device_id"),
        transport_listener(&tr_mock, &adapter_mock) {}

 protected:
  const int app_handle;
  const std::string dev_id;
  MockTransportManager tr_mock;
  TransportAdapterMock adapter_mock;
  TransportAdapterListenerImpl transport_listener;
};

MATCHER_P4(IsEvent, eventType, adapter, dev_id, app_id, "") {
  TransportAdapterEvent event = arg;
  return event.application_id == app_id && event.device_uid == dev_id &&
         event.event_type == eventType && event.transport_adapter == adapter;
}

MATCHER_P5(IsEvent, eventType, adapter, dev_id, app_id, data, "") {
  TransportAdapterEvent event = arg;
  return event.application_id == app_id && event.device_uid == dev_id &&
         event.event_type == eventType && event.transport_adapter == adapter &&
         event.event_data == data;
}

TEST_F(TransportAdapterListenerTest, OnCommunicationError) {
  EXPECT_CALL(
      tr_mock,
      ReceiveEventFromDevice(IsEvent(
          TransportAdapterListenerImpl::EventTypeEnum::ON_COMMUNICATION_ERROR,
          &adapter_mock, dev_id, app_handle))).WillOnce(Return(E_SUCCESS));
  transport_listener.OnCommunicationError(&adapter_mock, dev_id, app_handle);
}

TEST_F(TransportAdapterListenerTest, OnConnectDone) {
  EXPECT_CALL(tr_mock,
              ReceiveEventFromDevice(IsEvent(
                  TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE,
                  &adapter_mock, dev_id, app_handle)))
      .WillOnce(Return(E_SUCCESS));
  transport_listener.OnConnectDone(&adapter_mock, dev_id, app_handle);
}

TEST_F(TransportAdapterListenerTest, OnConnectFailed) {
  ConnectError er;

  EXPECT_CALL(tr_mock,
              ReceiveEventFromDevice(IsEvent(
                  TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL,
                  &adapter_mock, dev_id, app_handle)))
      .WillOnce(Return(E_SUCCESS));
  transport_listener.OnConnectFailed(&adapter_mock, dev_id, app_handle, er);
}

TEST_F(TransportAdapterListenerTest, OnDataReceiveDone) {
  ::protocol_handler::RawMessagePtr data_container;

  EXPECT_CALL(tr_mock,
              ReceiveEventFromDevice(IsEvent(
                  TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE,
                  &adapter_mock, dev_id, app_handle, data_container)))
      .WillOnce(Return(E_SUCCESS));
  transport_listener.OnDataReceiveDone(&adapter_mock, dev_id, app_handle,
                                       data_container);
}

TEST_F(TransportAdapterListenerTest, OnDataReceiveFailed) {
  DataReceiveError err;

  EXPECT_CALL(tr_mock,
              ReceiveEventFromDevice(IsEvent(
                  TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_FAIL,
                  &adapter_mock, dev_id, app_handle)))
      .WillOnce(Return(E_SUCCESS));
  transport_listener.OnDataReceiveFailed(&adapter_mock, dev_id, app_handle,
                                         err);
}

TEST_F(TransportAdapterListenerTest, OnDataSendDone) {
  unsigned char data[3] = {0x20, 0x07, 0x01};
  ::protocol_handler::RawMessagePtr data_container =
      new ::protocol_handler::RawMessage(1, 1, data, 3);

  EXPECT_CALL(tr_mock,
              ReceiveEventFromDevice(IsEvent(
                  TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE,
                  &adapter_mock, dev_id, app_handle, data_container)))
      .WillOnce(Return(E_SUCCESS));
  transport_listener.OnDataSendDone(&adapter_mock, dev_id, app_handle,
                                    data_container);
}

TEST_F(TransportAdapterListenerTest, OnDataSendFailed) {
  unsigned char data[3] = {0x20, 0x07, 0x01};
  ::protocol_handler::RawMessagePtr data_container =
      new ::protocol_handler::RawMessage(1, 1, data, 3);
  DataSendError err;

  EXPECT_CALL(tr_mock,
              ReceiveEventFromDevice(IsEvent(
                  TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL,
                  &adapter_mock, dev_id, app_handle, data_container)))
      .WillOnce(Return(E_SUCCESS));
  transport_listener.OnDataSendFailed(&adapter_mock, dev_id, app_handle,
                                      data_container, err);
}

TEST_F(TransportAdapterListenerTest, OnDeviceListUpdated) {
  EXPECT_CALL(
      tr_mock,
      ReceiveEventFromDevice(IsEvent(
          TransportAdapterListenerImpl::EventTypeEnum::ON_DEVICE_LIST_UPDATED,
          &adapter_mock, "", 0))).WillOnce(Return(E_SUCCESS));
  transport_listener.OnDeviceListUpdated(&adapter_mock);
}

TEST_F(TransportAdapterListenerTest, OnDisconnectDeviceDone) {
  EXPECT_CALL(
      tr_mock,
      ReceiveEventFromDevice(IsEvent(
          TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE,
          &adapter_mock, dev_id, app_handle))).WillOnce(Return(E_SUCCESS));
  transport_listener.OnDisconnectDone(&adapter_mock, dev_id, app_handle);
}

TEST_F(TransportAdapterListenerTest, OnDisconnectFailed) {
  DisconnectError err;

  EXPECT_CALL(
      tr_mock,
      ReceiveEventFromDevice(IsEvent(
          TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL,
          &adapter_mock, dev_id, app_handle))).WillOnce(Return(E_SUCCESS));
  transport_listener.OnDisconnectFailed(&adapter_mock, dev_id, app_handle, err);
}

TEST_F(TransportAdapterListenerTest, OnFindNewApplicationsRequest) {
  EXPECT_CALL(tr_mock, ReceiveEventFromDevice(IsEvent(
                           TransportAdapterListenerImpl::EventTypeEnum::
                               ON_FIND_NEW_APPLICATIONS_REQUEST,
                           &adapter_mock, "", 0))).WillOnce(Return(E_SUCCESS));
  transport_listener.OnFindNewApplicationsRequest(&adapter_mock);
}

TEST_F(TransportAdapterListenerTest, OnSearchDeviceDone) {
  EXPECT_CALL(tr_mock,
              ReceiveEventFromDevice(IsEvent(
                  TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE,
                  &adapter_mock, "", 0))).WillOnce(Return(E_SUCCESS));
  transport_listener.OnSearchDeviceDone(&adapter_mock);
}

TEST_F(TransportAdapterListenerTest, OnSearchDeviceFailed) {
  SearchDeviceError er;

  EXPECT_CALL(tr_mock,
              ReceiveEventFromDevice(IsEvent(
                  TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL,
                  &adapter_mock, "", 0))).WillOnce(Return(E_SUCCESS));
  transport_listener.OnSearchDeviceFailed(&adapter_mock, er);
}

TEST_F(TransportAdapterListenerTest, OnUnexpectedDisconnect) {
  CommunicationError err;

  EXPECT_CALL(
      tr_mock,
      ReceiveEventFromDevice(IsEvent(
          TransportAdapterListenerImpl::EventTypeEnum::ON_UNEXPECTED_DISCONNECT,
          &adapter_mock, dev_id, app_handle))).WillOnce(Return(E_SUCCESS));
  transport_listener.OnUnexpectedDisconnect(&adapter_mock, dev_id, app_handle,
                                            err);
}

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test
