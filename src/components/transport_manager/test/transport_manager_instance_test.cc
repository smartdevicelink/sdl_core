/*
 * Copyright (c) 2014, Ford Motor Company
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

/*
    Tests transport manager functionality with single device adapter that behaves correctly and single client
    Following sequence is tested:
        - TM created and runned
        - TM client registered as listener
        - TM client requests device scaning
        - single device was returned to TM client with onDeviceListUpdated callback
        - TM client calls "connect" on found device
        - device adapter sends onApplicationConnected
        - TM client receives onApplicationConnected
        - device adapter sends three data parts that represents single frame
        - TM client receives single frame with onFrameReceived callback
        - TM client calls sendFrame with some frame data and user data
        - TM client receives onFrameSendCompleted
        - TM client calls DisconnectDevice
        - TM client receives onApplicationDisconnected
 */

#include <stddef.h>
#include <stdio.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../../include/transport_manager/transport_manager.h"
#include "../include/transport_manager/transport_adapter/transport_adapter.h"
#include "../include/transport_manager/common.h"
#include "../include/transport_manager/transport_manager_impl.h"
#include "../include/transport_manager/transport_manager_default.h"
#include "../../connection_handler/include/connection_handler/connection_handler.h"


namespace test{
namespace test_transport_manager_instance {
TEST(test_transport_manager_instance, test_transport_manager_instance)
{
  transport_manager::TransportManager *Instance = transport_manager::TransportManagerDefault::instance();
  ASSERT_EQ(Instance, transport_manager::TransportManagerDefault::instance());
}

}}
