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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "hmi_message_handler/hmi_message_handler.h"
#include "hmi_message_handler/mqueue_adapter.h"

using hmi_message_handler::MessageSharedPointer;
using hmi_message_handler::HMIMessageHandler;
using hmi_message_handler::HMIMessageAdapter;
using hmi_message_handler::MqueueAdapter;
using application_manager::Message;

class MockHandler : public HMIMessageHandler {
 public:
  MOCK_METHOD1(OnMessageReceived, void(MessageSharedPointer message));
  MOCK_METHOD1(AddHMIMessageAdapter, void(HMIMessageAdapter* adapter));
  MOCK_METHOD1(RemoveHMIMessageAdapter, void(HMIMessageAdapter* adapter));
  MOCK_METHOD1(OnErrorSending, void(MessageSharedPointer message));
  MOCK_METHOD1(SendMessageToHMI, void(MessageSharedPointer message));
};

//TODO{ALeshin}: APPLINK-10846
//TEST(MqueueAdapter, Send) {
//  MockHandler handler;
//  HMIMessageAdapter* adapter = new MqueueAdapter(&handler);

//  MessageSharedPointer message(
//      new Message(protocol_handler::MessagePriority::kDefault));
//  message->set_json_message("{}");
//  adapter->SendMessageToHMI(message);

//  mqd_t mqd = mq_open("/sdl_to_hmi", O_RDONLY);
//  ASSERT_NE(-1, mqd);
//  static char buf[65536];
//  ssize_t sz = mq_receive(mqd, buf, 65536, NULL);
//  ASSERT_EQ(2, sz);
//  EXPECT_STREQ("{}", buf);

//  delete adapter;
//}

//TODO{ALeshin}: APPLINK-10846
//TEST(MqueueAdapter, Receive) {
//  MockHandler handler;
//  HMIMessageAdapter* adapter = new MqueueAdapter(&handler);

//  using ::testing::Property;
//  using ::testing::Pointee;
//  EXPECT_CALL(
//      handler,
//      OnMessageReceived(Property(
//          &MessageSharedPointer::get,
//          Pointee(Property(&Message::json_message, std::string("()"))))));

//  mqd_t mqd = mq_open("/hmi_to_sdl", O_WRONLY);
//  ASSERT_NE(-1, mqd);
//  const char buf[] = "()";
//  int rc = mq_send(mqd, buf, sizeof(buf) - 1, 0);
//  ASSERT_EQ(0, rc);

//  delete adapter;
//}
