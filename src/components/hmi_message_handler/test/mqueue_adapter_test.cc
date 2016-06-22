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

#include "gtest/gtest.h"

#include "hmi_message_handler/mqueue_adapter.h"
#include "hmi_message_handler/mock_hmi_message_handler.h"
#include "utils/make_shared.h"

namespace test {
namespace components {
namespace hmi_message_handler_test {

using hmi_message_handler::MessageSharedPointer;
using hmi_message_handler::MqueueAdapter;
using application_manager::Message;

// TODO {AKozoriz} : Will be fixed in planned task
TEST(MqueueAdapter, DISABLED_Send) {
  MockHMIMessageHandler handler;
  hmi_message_handler::HMIMessageAdapterImpl* adapter =
      new MqueueAdapter(&handler);

  MessageSharedPointer message(
      utils::MakeShared<Message>(protocol_handler::MessagePriority::kDefault));
  message->set_json_message("{}");
  adapter->SendMessageToHMI(message);

  mqd_t mqd = mq_open("/sdl_to_hmi", O_RDONLY);
  ASSERT_NE(-1, mqd);
  static char buf[65536];
  ssize_t sz = mq_receive(mqd, buf, 65536, NULL);
  ASSERT_EQ(2, sz);
  EXPECT_STREQ("{}", buf);

  delete adapter;
}

TEST(MqueueAdapter, DISABLED_Receive) {
  MockHMIMessageHandler handler;
  hmi_message_handler::HMIMessageAdapterImpl* adapter =
      new MqueueAdapter(&handler);

  using ::testing::Property;
  using ::testing::Pointee;
  EXPECT_CALL(
      handler,
      OnMessageReceived(Property(
          &MessageSharedPointer::get,
          Pointee(Property(&Message::json_message, std::string("()"))))));

  mqd_t mqd = mq_open("/hmi_to_sdl", O_WRONLY);
  ASSERT_NE(-1, mqd);
  const char buf[] = "()";
  int rc = mq_send(mqd, buf, sizeof(buf) - 1, 0);
  ASSERT_EQ(0, rc);

  delete adapter;
}

}  // namespace hmi_message_handler_test
}  // namespace components
}  // namespace test
