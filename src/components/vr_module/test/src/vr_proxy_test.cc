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

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "vr_module/vr_proxy.h"
#include "mock_vr_proxy_listener.h"
#include "mock_channel.h"

using ::testing::Return;

namespace vr_module {

class VRProxyTest : public ::testing::Test {
 protected:
};

TEST_F(VRProxyTest, SizeToString) {
  MockVRProxyListener *listener = new MockVRProxyListener();
  MockChannel *channel = new MockChannel();
  EXPECT_CALL(*channel, Start()).Times(1).WillOnce(Return(false));
  EXPECT_CALL(*channel, Stop()).Times(1).WillOnce(Return(false));
  VRProxy proxy(listener, channel);

  unsigned char value[] = { 123, 0, 0, 0 };
  std::string expected(value, value + 4);
  std::string output = proxy.SizeToString(123);

  EXPECT_EQ(expected.size(), output.size());
  EXPECT_EQ(expected, output);

  unsigned char value2[] = { 0xBD, 0x3, 0, 0 };
  std::string expected2(value2, value2 + 4);
  std::string output2 = proxy.SizeToString(957);

  EXPECT_EQ(expected2.size(), output2.size());
  EXPECT_EQ(expected2, output2);
}

TEST_F(VRProxyTest, SizeFromString) {
  MockVRProxyListener *listener = new MockVRProxyListener();
  MockChannel *channel = new MockChannel();
  EXPECT_CALL(*channel, Start()).Times(1).WillOnce(Return(false));
  EXPECT_CALL(*channel, Stop()).Times(1).WillOnce(Return(false));
  VRProxy proxy(listener, channel);

  unsigned char value[] = { 123, 0, 0, 0 };
  std::string input(value, value + 4);

  EXPECT_EQ(int32_t(123), proxy.SizeFromString(input));

  unsigned char value2[] = { 0xBD, 0x3, 0, 0 };
  std::string input2(value2, value2 + 4);

  EXPECT_EQ(int32_t(957), proxy.SizeFromString(input2));
}

}  // namespace vr_module
