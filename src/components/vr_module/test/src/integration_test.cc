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
#include <time.h>

#include <algorithm>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock_application.h"
#include "mock_connected_socket.h"
#include "mock_plugin_sender.h"
#include "mock_service.h"
#include "protocol/common.h"
#include "vr_module/interface/hmi.pb.h"
#include "vr_module/interface/mobile.pb.h"
#include "vr_module/socket_channel.h"
#include "vr_module/vr_module.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::AnyOf;

static const size_t kHeaderSize = 4;
static const size_t kChunkSize = 1000;

MATCHER_P(RawDataEq, message, ""){
std::string str;
message.SerializeToString(&str);
const UInt8* begin = reinterpret_cast<const UInt8*>(str.c_str());
const UInt8* end = reinterpret_cast<const UInt8*>(str.c_str() + str.size());
const UInt8* output = arg + kHeaderSize;
return std::equal(begin, end, output);
}

MATCHER_P2(RawMessageEq, service, message, ""){
const size_t size = message.ByteSize();
uint8_t data[size];
message.SerializeToArray(data, size);
return arg->connection_key() == uint32_t(service)
&& arg->protocol_version() == uint32_t(protocol_handler::PROTOCOL_VERSION_2)
&& arg->service_type() == uint32_t(protocol_handler::ServiceType::kVr)
&& arg->data_size() == size
&& std::equal(data, data + size, arg->data());
}

ACTION_P2(RecvRawMessage, module, message){
uint32_t size = message.ByteSize();
uint8_t data[size];
message.SerializeToArray(data, size);
protocol_handler::RawMessagePtr raw(new protocol_handler::RawMessage(1,
    protocol_handler::PROTOCOL_VERSION_2, data, size,
    protocol_handler::ServiceType::kVr, size));
module->ProcessMessageFromRemoteMobileService(raw);
}

ACTION_P(RecvHeader, header){
UInt8* buffer = const_cast<UInt8*>(arg0);
std::copy(header, header + kHeaderSize, buffer);
return kHeaderSize;
}

ACTION_P(RecvMessage, message){
UInt8* buffer = const_cast<UInt8*>(arg0);
std::string str;
message.SerializeToString(&str);
size_t size = str.size();
const UInt8* begin = reinterpret_cast<const UInt8*>(str.c_str());
const UInt8* end = reinterpret_cast<const UInt8*>(str.c_str() + size);
std::copy(begin, end, buffer);
return size;
}

ACTION(SizeHeader){
return kHeaderSize;
}

ACTION_P(SizeMessage, message){
return message.ByteSize() + kHeaderSize;
}

ACTION_P(Lock, mutex){
mutex->Lock();
return 0;
}

ACTION_P(Unlock, mutex){
mutex->Unlock();
}

namespace vr_module {

struct Mutex {
  pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
  void Lock() {
    pthread_mutex_lock(&m);
  }
  void Try(int seconds = 0) {
    if (seconds < 1) {
      timespec timestamp;
      clock_gettime(CLOCK_REALTIME, &timestamp);
      timestamp.tv_sec += seconds;
      pthread_mutex_timedlock(&m, &timestamp);
    } else {
      pthread_mutex_trylock(&m);
    }
  }
  void Unlock() {
    pthread_mutex_unlock(&m);
  }
};

class IntegrationTest : public ::testing::Test {
 protected:
};

TEST_F(IntegrationTest, SupportService) {
  MockPluginSender sender;
  net::MockConnectedSocket* socket = new net::MockConnectedSocket();
  VRModule module(&sender, new SocketChannel(socket));
  Mutex mtx, mtx_net;

  vr_hmi_api::ServiceMessage support_out;
  support_out.set_rpc(vr_hmi_api::SUPPORT_SERVICE);
  support_out.set_rpc_type(vr_hmi_api::REQUEST);
  support_out.set_correlation_id(1);
  EXPECT_CALL(*socket, send(RawDataEq(support_out),
          kHeaderSize + support_out.ByteSize(), _)).Times(1).WillOnce(
      DoAll(Unlock(&mtx), SizeMessage(support_out)));

  vr_hmi_api::ServiceMessage support_in;
  support_in.set_rpc(vr_hmi_api::SUPPORT_SERVICE);
  support_in.set_rpc_type(vr_hmi_api::RESPONSE);
  support_in.set_correlation_id(1);
  vr_hmi_api::SupportServiceResponse body;
  body.set_result(vr_hmi_api::SUCCESS);
  std::string params;
  body.SerializeToString(&params);
  support_in.set_params(params);
  UInt8 header[kHeaderSize] = { 0, 0, 0, 10 };
  EXPECT_CALL(*socket, recv(_, AnyOf(kHeaderSize, support_in.ByteSize()), _)).
      Times(3).WillOnce(RecvHeader(header)).WillOnce(RecvMessage(support_in))
      .WillOnce(Lock(&mtx_net));
  EXPECT_CALL(*socket, shutdown()).Times(1).WillOnce(Unlock(&mtx_net));
  EXPECT_CALL(*socket, close()).Times(1);

  EXPECT_FALSE(module.IsSupported());
  mtx_net.Lock();
  module.Start();
  module.CheckSupport();
  mtx.Try(10);
  sleep(1);
  EXPECT_TRUE(module.IsSupported());
}

TEST_F(IntegrationTest, OnRegisterService) {
  MockPluginSender sender;
  net::MockConnectedSocket* socket = new net::MockConnectedSocket();
  VRModule module(&sender, new SocketChannel(socket));

  application_manager::MockService* service =
      new application_manager::MockService();
  application_manager::ServicePtr service_ptr(service);
  module.set_service(service_ptr);

  application_manager::MockApplication* app =
      new application_manager::MockApplication();
  application_manager::ApplicationSharedPtr app_ptr(app);
  EXPECT_CALL(*service, GetApplication(1)).Times(1).WillOnce(Return(app_ptr));
  EXPECT_CALL(*app, mobile_app_id()).Times(1).WillOnce(Return("app"));

  vr_hmi_api::ServiceMessage on_register_out;
  on_register_out.set_rpc(vr_hmi_api::ON_REGISTER);
  on_register_out.set_rpc_type(vr_hmi_api::NOTIFICATION);
  on_register_out.set_correlation_id(1);
  vr_hmi_api::OnRegisterServiceNotification notification;
  notification.set_appid(1);
  notification.set_default_(false);
  std::string params;
  notification.SerializeToString(&params);
  on_register_out.set_params(params);
  EXPECT_CALL(*socket, send(RawDataEq(on_register_out),
          kHeaderSize + on_register_out.ByteSize(), _)).Times(1).WillOnce(
      SizeMessage(on_register_out));
  EXPECT_CALL(*socket, close()).Times(1);

  module.supported_ = true;
  module.OnServiceStartedCallback(1, "device");
}

TEST_F(IntegrationTest, ActivateService) {
  MockPluginSender sender;
  net::MockConnectedSocket* socket = new net::MockConnectedSocket();
  VRModule module(&sender, new SocketChannel(socket));
  Mutex mtx, mtx_net;

  vr_hmi_api::ServiceMessage activate_in;
  activate_in.set_rpc(vr_hmi_api::ACTIVATE);
  activate_in.set_rpc_type(vr_hmi_api::REQUEST);
  activate_in.set_correlation_id(10);
  vr_hmi_api::ActivateServiceRequest request;
  request.set_appid(1);
  std::string params;
  request.SerializeToString(&params);
  activate_in.set_params(params);
  UInt8 header[kHeaderSize] = { 0, 0, 0, 10 };
  EXPECT_CALL(*socket, recv(_, AnyOf(kHeaderSize, activate_in.ByteSize()), _)).
      Times(3).WillOnce(RecvHeader(header)).WillOnce(RecvMessage(activate_in))
      .WillOnce(Lock(&mtx_net));

  vr_mobile_api::ServiceMessage activate_to;
  activate_to.set_rpc(vr_mobile_api::ACTIVATE);
  activate_to.set_rpc_type(vr_mobile_api::REQUEST);
  activate_to.set_correlation_id(1);

  vr_mobile_api::ServiceMessage activate_from;
  activate_from.set_rpc(vr_mobile_api::ACTIVATE);
  activate_from.set_rpc_type(vr_mobile_api::RESPONSE);
  activate_from.set_correlation_id(1);
  vr_mobile_api::ActivateServiceResponse response;
  response.set_result(vr_mobile_api::SUCCESS);
  std::string params2;
  response.SerializeToString(&params2);
  activate_from.set_params(params2);
  EXPECT_CALL(sender, SendMessageToRemoteMobileService(
          RawMessageEq(1, activate_to))).Times(1).WillOnce(
      RecvRawMessage(&module, activate_from));

  vr_hmi_api::ServiceMessage activate_out;
  activate_out.set_rpc(vr_hmi_api::ACTIVATE);
  activate_out.set_rpc_type(vr_hmi_api::RESPONSE);
  activate_out.set_correlation_id(10);
  vr_hmi_api::ActivateServiceResponse response2;
  response2.set_result(vr_hmi_api::SUCCESS);
  std::string params3;
  response2.SerializeToString(&params3);
  activate_out.set_params(params3);
  EXPECT_CALL(*socket, send(RawDataEq(activate_out),
          kHeaderSize + activate_out.ByteSize(), _)
  ).Times(1).WillOnce(DoAll(Unlock(&mtx), SizeMessage(activate_out)));
  EXPECT_CALL(*socket, shutdown()).Times(1).WillOnce(Unlock(&mtx_net));
  EXPECT_CALL(*socket, close()).Times(1);

  mtx.Lock();
  module.supported_ = true;
  EXPECT_FALSE(module.HasActivatedService());
  mtx_net.Lock();
  module.Start();
  mtx.Try(10);
  sleep(1);
  EXPECT_TRUE(module.HasActivatedService());
  EXPECT_EQ(1, module.active_service_);
}

TEST_F(IntegrationTest, ProcessData) {
  MockPluginSender sender;
  net::MockConnectedSocket* socket = new net::MockConnectedSocket();
  VRModule module(&sender, new SocketChannel(socket));
  Mutex mtx, mtx_net;

  vr_hmi_api::ServiceMessage data_in;
  data_in.set_rpc(vr_hmi_api::PROCESS_DATA);
  data_in.set_rpc_type(vr_hmi_api::REQUEST);
  data_in.set_correlation_id(10);
  vr_hmi_api::ProcessDataRequest request;
  request.set_raw("0123456789");
  std::string params;
  request.SerializeToString(&params);
  data_in.set_params(params);
  UInt8 header[kHeaderSize] = { 0, 0, 0, 20 };
  EXPECT_CALL(*socket, recv(_, AnyOf(kHeaderSize, data_in.ByteSize()), _)).
      Times(3).WillOnce(RecvHeader(header)).WillOnce(RecvMessage(data_in))
      .WillOnce(Lock(&mtx_net));

  vr_mobile_api::ServiceMessage data_to;
  data_to.set_rpc(vr_mobile_api::PROCESS_DATA);
  data_to.set_rpc_type(vr_mobile_api::REQUEST);
  data_to.set_correlation_id(1);
  vr_mobile_api::ProcessDataRequest request2;
  request2.set_raw("0123456789");
  std::string params2;
  request2.SerializeToString(&params2);
  data_to.set_params(params2);

  vr_mobile_api::ServiceMessage data_from;
  data_from.set_rpc(vr_mobile_api::PROCESS_DATA);
  data_from.set_rpc_type(vr_mobile_api::RESPONSE);
  data_from.set_correlation_id(1);
  vr_mobile_api::ProcessDataResponse response;
  response.set_result(vr_mobile_api::SUCCESS);
  std::string params3;
  response.SerializeToString(&params3);
  data_from.set_params(params3);
  EXPECT_CALL(sender, SendMessageToRemoteMobileService(
          RawMessageEq(1, data_to))).Times(1).WillOnce(
      RecvRawMessage(&module, data_from));

  vr_hmi_api::ServiceMessage data_out;
  data_out.set_rpc(vr_hmi_api::PROCESS_DATA);
  data_out.set_rpc_type(vr_hmi_api::RESPONSE);
  data_out.set_correlation_id(10);
  vr_hmi_api::ProcessDataResponse response2;
  response2.set_result(vr_hmi_api::SUCCESS);
  std::string params4;
  response2.SerializeToString(&params4);
  data_out.set_params(params4);
  EXPECT_CALL(*socket, send(RawDataEq(data_out),
          kHeaderSize + data_out.ByteSize(), _)
  ).Times(1).WillOnce(DoAll(Unlock(&mtx), SizeMessage(data_out)));
  EXPECT_CALL(*socket, shutdown()).Times(1).WillOnce(Unlock(&mtx_net));
  EXPECT_CALL(*socket, close()).Times(1);

  mtx.Lock();
  module.supported_ = true;
  module.active_service_ = 1;
  mtx_net.Lock();
  module.Start();
  mtx.Try(10);
  sleep(1);
}

TEST_F(IntegrationTest, OnDefaultServiceChosen) {
  MockPluginSender sender;
  net::MockConnectedSocket* socket = new net::MockConnectedSocket();
  VRModule module(&sender, new SocketChannel(socket));
  Mutex mtx;

  vr_hmi_api::ServiceMessage on_default_in;
  on_default_in.set_rpc(vr_hmi_api::ON_DEFAULT_CHOSEN);
  on_default_in.set_rpc_type(vr_hmi_api::NOTIFICATION);
  on_default_in.set_correlation_id(10);
  vr_hmi_api::OnDefaultServiceChosenNotification notification;
  notification.set_appid(1);
  std::string params;
  notification.SerializeToString(&params);
  on_default_in.set_params(params);
  UInt8 header[kHeaderSize] = { 0, 0, 0, 10 };
  EXPECT_CALL(*socket, recv(_, AnyOf(kHeaderSize, on_default_in.ByteSize()), _))
      .Times(3).WillOnce(RecvHeader(header)).WillOnce(
      RecvMessage(on_default_in)).WillOnce(Lock(&mtx));
  EXPECT_CALL(*socket, shutdown()).Times(1).WillOnce(Unlock(&mtx));
  EXPECT_CALL(*socket, close()).Times(1);

  module.supported_ = true;
  module.services_[1] = {"app", "dev"};
  mtx.Lock();
  module.Start();
  sleep(1);
  EXPECT_TRUE(module.IsDefaultService(1));
}

TEST_F(IntegrationTest, OnDeactivateService) {
  MockPluginSender sender;
  net::MockConnectedSocket* socket = new net::MockConnectedSocket();
  VRModule module(&sender, new SocketChannel(socket));
  Mutex mtx, mtx_net;

  vr_hmi_api::ServiceMessage on_deactivate_in;
  on_deactivate_in.set_rpc(vr_hmi_api::ON_DEACTIVATED);
  on_deactivate_in.set_rpc_type(vr_hmi_api::NOTIFICATION);
  on_deactivate_in.set_correlation_id(10);
  UInt8 header[kHeaderSize] = { 0, 0, 0, 6 };
  EXPECT_CALL(*socket, recv(_, AnyOf(kHeaderSize, on_deactivate_in.ByteSize()), _))
      .Times(3).WillOnce(RecvHeader(header)).WillOnce(
      RecvMessage(on_deactivate_in)).WillOnce(Lock(&mtx_net));

  vr_mobile_api::ServiceMessage on_deactivate_to;
  on_deactivate_to.set_rpc(vr_mobile_api::ON_DEACTIVATED);
  on_deactivate_to.set_rpc_type(vr_mobile_api::NOTIFICATION);
  on_deactivate_to.set_correlation_id(1);
  EXPECT_CALL(sender, SendMessageToRemoteMobileService(
          RawMessageEq(1, on_deactivate_to))).Times(1).WillOnce(Unlock(&mtx));
  EXPECT_CALL(*socket, shutdown()).Times(1).WillOnce(Unlock(&mtx_net));
  EXPECT_CALL(*socket, close()).Times(1);

  mtx.Lock();
  module.supported_ = true;
  module.active_service_ = 1;
  EXPECT_TRUE(module.HasActivatedService());
  mtx_net.Lock();
  module.Start();
  mtx.Try(10);
  sleep(1);
  EXPECT_FALSE(module.HasActivatedService());
  EXPECT_EQ(0, module.active_service_);
}

}  // namespace vr_module
