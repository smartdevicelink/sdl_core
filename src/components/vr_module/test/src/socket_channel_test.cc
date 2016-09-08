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

#include <algorithm>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "mock_connected_socket.h"
#include "vr_module/socket_channel.h"

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArrayArgument;
using vr_module::SocketChannel;

MATCHER_P2(RawDataEq, expected, size, ""){
  return std::equal(expected, expected + size, arg);
}

namespace net {

class SocketChannelTest : public ::testing::Test {
 protected:
};

TEST_F(SocketChannelTest, Start) {
  MockConnectedSocket* socket = new MockConnectedSocket();
  SocketChannel channel(socket);
  EXPECT_CALL(*socket, close()).Times(1);
  EXPECT_TRUE(channel.Start());
}

TEST_F(SocketChannelTest, Stop) {
  MockConnectedSocket* socket = new MockConnectedSocket();
  SocketChannel channel(socket);
  EXPECT_CALL(*socket, shutdown()).Times(1);
  EXPECT_CALL(*socket, close()).Times(1);
  EXPECT_TRUE(channel.Stop());
}

TEST_F(SocketChannelTest, SendLessChunk) {
  MockConnectedSocket* socket = new MockConnectedSocket();
  SocketChannel channel(socket);
  EXPECT_CALL(*socket, close()).Times(1);
  std::string input(999, 'x');
  size_t size = input.size();
  const UInt8* data = reinterpret_cast<const UInt8*>(input.c_str());
  EXPECT_CALL(*socket, send(RawDataEq(data, size), size, _)).Times(1).WillOnce(
      Return(size));
  EXPECT_TRUE(channel.Send(input));
}

TEST_F(SocketChannelTest, SendChunk) {
  MockConnectedSocket* socket = new MockConnectedSocket();
  SocketChannel channel(socket);
  EXPECT_CALL(*socket, close()).Times(1);
  std::string input(1000, 'x');
  const size_t kChunkSize = 1000;
  const UInt8* data = reinterpret_cast<const UInt8*>(input.c_str());
  EXPECT_CALL(*socket, send(RawDataEq(data, kChunkSize), kChunkSize, _)).Times(
      1).WillOnce(Return(kChunkSize));
  EXPECT_TRUE(channel.Send(input));
}

TEST_F(SocketChannelTest, SendMoreChunk) {
  MockConnectedSocket* socket = new MockConnectedSocket();
  SocketChannel channel(socket);
  EXPECT_CALL(*socket, close()).Times(1);
  std::string input(1001, 'x');
  const size_t kChunkSize = 1000;
  size_t size = input.size() - kChunkSize;
  const UInt8* data1 = reinterpret_cast<const UInt8*>(input.c_str());
  const UInt8* data2 =
      reinterpret_cast<const UInt8*>(input.c_str() + kChunkSize);
  EXPECT_CALL(*socket, send(RawDataEq(data1, kChunkSize), kChunkSize, _)).Times(
      1).WillOnce(Return(kChunkSize));
  EXPECT_CALL(*socket, send(RawDataEq(data2, size), size, _)).Times(1).WillOnce(
      Return(size));
  EXPECT_TRUE(channel.Send(input));
}

TEST_F(SocketChannelTest, ReceiveLessChunk) {
  MockConnectedSocket* socket = new MockConnectedSocket();
  SocketChannel channel(socket);
  EXPECT_CALL(*socket, close()).Times(1);
  std::string input(999, 'x');
  size_t size = input.size();
  const UInt8* data = reinterpret_cast<const UInt8*>(input.c_str());
  EXPECT_CALL(*socket, recv(_, size, _)).Times(1).WillOnce(
      DoAll(SetArrayArgument<0>(data, data + size), Return(size)));
  std::string expected(999, 'x');
  std::string output;
  EXPECT_TRUE(channel.Receive(size, &output));
  EXPECT_EQ(expected, output);
}

TEST_F(SocketChannelTest, ReceiveChunk) {
  MockConnectedSocket* socket = new MockConnectedSocket();
  SocketChannel channel(socket);
  EXPECT_CALL(*socket, close()).Times(1);
  const size_t kChunkSize = 1000;
  std::string input(kChunkSize, 'x');
  const UInt8* data = reinterpret_cast<const UInt8*>(input.c_str());
  EXPECT_CALL(*socket, recv(_, kChunkSize, _)).Times(1).WillOnce(
      DoAll(SetArrayArgument<0>(data, data + kChunkSize), Return(kChunkSize)));
  std::string expected(kChunkSize, 'x');
  std::string output;
  EXPECT_TRUE(channel.Receive(kChunkSize, &output));
  EXPECT_EQ(expected, output);
}

TEST_F(SocketChannelTest, ReceiveMoreChunk) {
  MockConnectedSocket* socket = new MockConnectedSocket();
  SocketChannel channel(socket);
  EXPECT_CALL(*socket, close()).Times(1);
  std::string input(1001, 'x');
  const size_t kChunkSize = 1000;
  size_t size = input.size() - kChunkSize;
  const UInt8* data1 = reinterpret_cast<const UInt8*>(input.c_str());
  const UInt8* data2 =
      reinterpret_cast<const UInt8*>(input.c_str() + kChunkSize);
  EXPECT_CALL(*socket, recv(_, kChunkSize, _)).Times(1).WillOnce(
      DoAll(SetArrayArgument<0>(data1, data1 + kChunkSize),
            Return(kChunkSize)));
  EXPECT_CALL(*socket, recv(_, size, _)).Times(1).WillOnce(
      DoAll(SetArrayArgument<0>(data2, data2 + size), Return(size)));
  std::string expected(input.size(), 'x');
  std::string output;
  EXPECT_TRUE(channel.Receive(input.size(), &output));
  EXPECT_EQ(expected, output);
}

}  // namespace net
