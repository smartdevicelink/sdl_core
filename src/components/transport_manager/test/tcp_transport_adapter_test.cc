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
#include "transport_manager/tcp/tcp_transport_adapter.h"
#include "transport_manager/transport_adapter/connection.h"
#include "config_profile/profile.h"
#include "resumption/last_state.h"
#include "protocol/raw_message.h"
#include "include/transport_adapter_listener_mock.h"
#include "include/device_mock.h"
#include "include/connection_mock.h"

namespace test {
namespace components {
namespace transport_manager_test {

using ::testing::Return;
using ::testing::_;

using namespace ::protocol_handler;
using namespace ::transport_manager;
using namespace transport_manager::transport_adapter;

class TestTCPTransportAdapter : public TcpTransportAdapter {
 public:
  TestTCPTransportAdapter(uint16_t port) : TcpTransportAdapter(port) {
    ::profile::Profile::instance()->config_file_name(
        "smartDeviceLink_test.ini");
  }
  MOCK_CONST_METHOD2(FindEstablishedConnection,
                     ConnectionSPtr(const DeviceUID& device_handle,
                                    const ApplicationHandle& app_handle));

  MOCK_CONST_METHOD1(FindDevice, DeviceSptr(const DeviceUID& device_handle));
  MOCK_METHOD2(Connect,
               TransportAdapter::Error(const DeviceUID& device_handle,
                                       const ApplicationHandle& app_handle));
  void CallStore() { Store(); }
  bool CallRestore() { return Restore(); }
};

class TcpAdapterTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {
    ::profile::Profile::instance()->config_file_name(
        "smartDeviceLink_test.ini");
  }
   virtual void SetUp() {
       resumption::LastState::instance()->dictionary = Json::Value();
  }

  virtual void TearDown() { resumption::LastState::destroy(); }

  const uint32_t port = 12345;
  const std::string string_port = "12345";
};

TEST_F(TcpAdapterTest, StoreDataWithOneDeviceAndOneApplication) {
  // Prepare
  TestTCPTransportAdapter transport_adapter(port);
  std::string uniq_id = "unique_device_name";
  utils::SharedPtr<TCPDeviceMock> mockdev = new TCPDeviceMock(port, uniq_id);
  transport_adapter.AddDevice(mockdev);

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(1u, devList.size());
  EXPECT_EQ(uniq_id, devList[0]);

  const int app_handle = 1;
  std::vector<int> intList = {app_handle};
  EXPECT_CALL(*mockdev, GetApplicationList()).WillOnce(Return(intList));

  ConnectionSPtr mock_connection = new ConnectionMock();
  EXPECT_CALL(transport_adapter, FindDevice(uniq_id)).WillOnce(Return(mockdev));
  EXPECT_CALL(transport_adapter, FindEstablishedConnection(uniq_id, app_handle))
      .WillOnce(Return(mock_connection));

  EXPECT_CALL(*mockdev, GetApplicationPort(app_handle)).WillOnce(Return(port));

  transport_adapter.CallStore();

  // Check that value is saved
  Json::Value& tcp_dict = resumption::LastState::instance()
                              ->dictionary["TransportManager"]["TcpAdapter"];

  ASSERT_TRUE(tcp_dict.isObject());
  ASSERT_FALSE(tcp_dict["devices"].isNull());
  ASSERT_FALSE(tcp_dict["devices"][0]["applications"].isNull());
  ASSERT_FALSE(tcp_dict["devices"][0]["address"].isNull());
  EXPECT_EQ(1u, tcp_dict["devices"][0]["applications"].size());
  EXPECT_EQ(string_port,
            tcp_dict["devices"][0]["applications"][0]["port"].asString());
  EXPECT_EQ(uniq_id, tcp_dict["devices"][0]["name"].asString());
}

TEST_F(TcpAdapterTest, StoreDataWithSeveralDevicesAndOneApplication) {
  // Prepare
  TestTCPTransportAdapter transport_adapter(port);
  const uint32_t count_dev = 10;
  utils::SharedPtr<TCPDeviceMock> mockdev[count_dev];
  std::string uniq_id[count_dev];
  for (uint32_t i = 0; i < count_dev; i++) {
    char numb[12];
    std::snprintf(numb, 12, "%d", i);
    uniq_id[i] = "unique_device_name" + std::string(numb);
    mockdev[i] = new TCPDeviceMock(port, uniq_id[i]);
    EXPECT_CALL(*(mockdev[i]), IsSameAs(_)).WillRepeatedly(Return(false));
    transport_adapter.AddDevice(mockdev[i]);
  }

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(count_dev, devList.size());
  EXPECT_EQ(uniq_id[0], devList[0]);

  const int app_handle = 1;
  std::vector<int> intList = {app_handle};

  ConnectionSPtr mock_connection = new ConnectionMock();
  for (uint32_t i = 0; i < count_dev; i++) {
    EXPECT_CALL(transport_adapter, FindDevice(uniq_id[i]))
        .WillOnce(Return(mockdev[i]));
    EXPECT_CALL(*(mockdev[i]), GetApplicationList()).WillOnce(Return(intList));

    EXPECT_CALL(transport_adapter,
                FindEstablishedConnection(uniq_id[i], app_handle))
        .WillOnce(Return(mock_connection));

    EXPECT_CALL(*(mockdev[i]), GetApplicationPort(app_handle))
        .WillOnce(Return(port));
  }
  transport_adapter.CallStore();

  // Check that values are saved
  Json::Value& tcp_dict = resumption::LastState::instance()
                              ->dictionary["TransportManager"]["TcpAdapter"];
  ASSERT_TRUE(tcp_dict.isObject());
  ASSERT_FALSE(tcp_dict["devices"].isNull());
  for (uint32_t i = 0; i < count_dev; i++) {
    ASSERT_FALSE(tcp_dict["devices"][i]["applications"].isNull());
    ASSERT_FALSE(tcp_dict["devices"][i]["address"].isNull());
    EXPECT_EQ(1u, tcp_dict["devices"][i]["applications"].size());
    EXPECT_EQ(string_port,
              tcp_dict["devices"][i]["applications"][0]["port"].asString());
    EXPECT_EQ(uniq_id[i], tcp_dict["devices"][i]["name"].asString());
  }
}

TEST_F(TcpAdapterTest, StoreDataWithSeveralDevicesAndSeveralApplications) {
  // Prepare
  TestTCPTransportAdapter transport_adapter(port);
  const uint32_t count_dev = 10;

  utils::SharedPtr<TCPDeviceMock> mockdev[count_dev];
  std::string uniq_id[count_dev];
  for (uint32_t i = 0; i < count_dev; i++) {
    char numb[12];
    std::snprintf(numb, 12, "%d", i);
    uniq_id[i] = "unique_device_name" + std::string(numb);
    mockdev[i] = new TCPDeviceMock(port, uniq_id[i]);
    EXPECT_CALL(*(mockdev[i]), IsSameAs(_)).WillRepeatedly(Return(false));
    transport_adapter.AddDevice(mockdev[i]);
  }

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(count_dev, devList.size());
  EXPECT_EQ(uniq_id[0], devList[0]);

  const uint32_t connection_count = 3;
  const int app_handle[connection_count] = {1, 2, 3};
  std::vector<int> intList = {app_handle[0], app_handle[1], app_handle[2]};
  const std::string ports[connection_count] = {"11111", "67890", "98765"};
  const int int_port[connection_count] = {11111, 67890, 98765};
  ConnectionSPtr mock_connection = new ConnectionMock();
  for (uint32_t i = 0; i < count_dev; i++) {
    EXPECT_CALL(transport_adapter, FindDevice(uniq_id[i]))
        .WillOnce(Return(mockdev[i]));
    EXPECT_CALL(*(mockdev[i]), GetApplicationList()).WillOnce(Return(intList));

    for (uint32_t j = 0; j < connection_count; j++) {
      EXPECT_CALL(transport_adapter,
                  FindEstablishedConnection(uniq_id[i], app_handle[j]))
          .WillOnce(Return(mock_connection));
      EXPECT_CALL(*(mockdev[i]), GetApplicationPort(app_handle[j]))
          .WillOnce(Return(int_port[j]));
    }
  }
  transport_adapter.CallStore();

  // Check that value is saved
  Json::Value& tcp_dict = resumption::LastState::instance()
                              ->dictionary["TransportManager"]["TcpAdapter"];

  ASSERT_TRUE(tcp_dict.isObject());
  ASSERT_FALSE(tcp_dict["devices"].isNull());
  for (uint32_t i = 0; i < count_dev; i++) {
    ASSERT_FALSE(tcp_dict["devices"][i]["applications"].isNull());
    ASSERT_FALSE(tcp_dict["devices"][i]["address"].isNull());
    for (uint32_t j = 0; j < intList.size(); j++) {
      EXPECT_EQ(ports[j],
                tcp_dict["devices"][i]["applications"][j]["port"].asString());
      EXPECT_EQ(uniq_id[i], tcp_dict["devices"][i]["name"].asString());
    }
  }
}

TEST_F(TcpAdapterTest, StoreData_ConnectionNotExist_DataNotStored) {
  // Prepare
  TestTCPTransportAdapter transport_adapter(port);
  std::string uniq_id = "unique_device_name";
  utils::SharedPtr<TCPDeviceMock> mockdev = new TCPDeviceMock(port, uniq_id);
  transport_adapter.AddDevice(mockdev);

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(1u, devList.size());
  EXPECT_EQ(uniq_id, devList[0]);
  std::vector<int> intList = {};
  EXPECT_CALL(*mockdev, GetApplicationList()).WillOnce(Return(intList));

  EXPECT_CALL(transport_adapter, FindDevice(uniq_id)).WillOnce(Return(mockdev));
  EXPECT_CALL(transport_adapter, FindEstablishedConnection(uniq_id, _))
      .Times(0);
  EXPECT_CALL(*mockdev, GetApplicationPort(_)).Times(0);
  transport_adapter.CallStore();

  // Check that value is not saved
  Json::Value& tcp_dict =
      resumption::LastState::instance()
          ->dictionary["TransportManager"]["TcpAdapter"]["devices"];
  ASSERT_TRUE(tcp_dict.isNull());
}

TEST_F(TcpAdapterTest, RestoreData_DataNotStored) {
  Json::Value& tcp_adapter_dictionary =
      resumption::LastState::instance()
          ->dictionary["TransportManager"]["TcpAdapter"];
  tcp_adapter_dictionary = Json::Value();
  TestTCPTransportAdapter transport_adapter(port);
  EXPECT_CALL(transport_adapter, Connect(_, _)).Times(0);
  EXPECT_TRUE(transport_adapter.CallRestore());
}

TEST_F(TcpAdapterTest, StoreDataWithOneDevice_RestoreData) {
  TestTCPTransportAdapter transport_adapter(port);
  std::string uniq_id = "unique_device_name";
  utils::SharedPtr<TCPDeviceMock> mockdev = new TCPDeviceMock(port, uniq_id);
  transport_adapter.AddDevice(mockdev);

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(1u, devList.size());
  EXPECT_EQ(uniq_id, devList[0]);

  const int app_handle = 1;
  std::vector<int> intList = {app_handle};
  EXPECT_CALL(*mockdev, GetApplicationList()).WillOnce(Return(intList));

  ConnectionSPtr mock_connection = new ConnectionMock();
  EXPECT_CALL(transport_adapter, FindDevice(uniq_id)).WillOnce(Return(mockdev));
  EXPECT_CALL(transport_adapter, FindEstablishedConnection(uniq_id, app_handle))
      .WillOnce(Return(mock_connection));

  EXPECT_CALL(*mockdev, GetApplicationPort(app_handle)).WillOnce(Return(port));

  transport_adapter.CallStore();

  EXPECT_CALL(transport_adapter, Connect(uniq_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));

  EXPECT_TRUE(transport_adapter.CallRestore());

  devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(1u, devList.size());
  EXPECT_EQ(uniq_id, devList[0]);
}

TEST_F(TcpAdapterTest, StoreDataWithSeveralDevices_RestoreData) {
  TestTCPTransportAdapter transport_adapter(port);
  const uint32_t count_dev = 10;

  utils::SharedPtr<TCPDeviceMock> mockdev[count_dev];
  std::string uniq_id[count_dev];
  for (uint32_t i = 0; i < count_dev; i++) {
    char numb[12];
    std::snprintf(numb, 12, "%d", i);
    uniq_id[i] = "unique_device_name" + std::string(numb);
    mockdev[i] = new TCPDeviceMock(port, uniq_id[i]);
    EXPECT_CALL(*(mockdev[i]), IsSameAs(_)).WillRepeatedly(Return(false));
    transport_adapter.AddDevice(mockdev[i]);
  }

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(count_dev, devList.size());
  EXPECT_EQ(uniq_id[0], devList[0]);

  const int app_handle = 1;
  std::vector<int> intList = {app_handle};

  ConnectionSPtr mock_connection = new ConnectionMock();
  for (uint32_t i = 0; i < count_dev; i++) {
    EXPECT_CALL(transport_adapter, FindDevice(uniq_id[i]))
        .WillOnce(Return(mockdev[i]));
    EXPECT_CALL(*(mockdev[i]), GetApplicationList()).WillOnce(Return(intList));

    EXPECT_CALL(transport_adapter,
                FindEstablishedConnection(uniq_id[i], app_handle))
        .WillOnce(Return(mock_connection));

    EXPECT_CALL(*(mockdev[i]), GetApplicationPort(app_handle))
        .WillOnce(Return(port));
  }
  transport_adapter.CallStore();

  for (uint32_t i = 0; i < count_dev; i++) {
    EXPECT_CALL(transport_adapter, Connect(uniq_id[i], app_handle))
        .WillOnce(Return(TransportAdapter::OK));
  }

  EXPECT_TRUE(transport_adapter.CallRestore());

  devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(count_dev, devList.size());
  for (uint32_t i = 0; i < count_dev; i++) {
    EXPECT_EQ(uniq_id[i], devList[i]);
  }
}

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test
