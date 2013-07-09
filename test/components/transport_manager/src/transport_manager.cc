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
using ::testing::AtLeast;

TEST(TransportManagerImpl, instance)
{
	TransportManagerImpl* prev_impl = TransportManagerImpl::instance();
	ASSERT_EQ(prev_impl, TransportManagerImpl::instance());
}

TEST(TransportManagerImpl, searchDevice)
{
  TransportManagerImpl* impl = TransportManagerImpl::instance();

  MockDeviceAdapter *mock_da = new MockDeviceAdapter();
  impl->addDeviceAdapter(mock_da);

  MockTransportManagerListener *tml = new MockTransportManagerListener();
  impl->addEventListener(tml);
  impl->addAdapterListener(mock_da, new DeviceAdapterListenerImpl(impl));

  //MockDeviceAdapterListener *mdal = new MockDeviceAdapterListener();

  //impl->addAdapterListener(mock_da, mdal);

  mock_da->init(new DeviceHandleGeneratorImpl(),
                NULL);

  impl->searchDevices();

  EXPECT_CALL(*tml, onSearchDeviceDone(_, _)).Times(AtLeast(1));
  EXPECT_CALL(*tml, onSearchDeviceFailed(_, _)).Times(AtLeast(0));


//  EXPECT_CALL(*mdal, onConnectDone(_, _)).Times(1);
//  EXPECT_CALL(*mdal, onConnectFailed(_, _, _)).Times(1);
//
//  impl->connectDevice(1, 1, 1);
//  unsigned char buf[10] = { 0 };
//
//  RawMessageSptr msg(new RawMessage(10, 1, 1, buf, 10));
//
//  impl->sendMessageToDevice(msg);
//
//  EXPECT_CALL(*mdal, onDataSendDone(_, _, _)).Times(1);
//  EXPECT_CALL(*mdal, onDataSendFailed(_, _, _, _)).Times(1);
}
