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

#include <unistd.h>
#include "gtest/gtest.h"
#include "utils/message_queue.h"

namespace test {
namespace components {
namespace utils {

using ::utils::MessageQueue;

class MessageQueueTest : public testing::Test {
 public:
  MessageQueueTest()
      : test_val_1("Hello,"),
        test_val_2("Beautiful "),
        test_val_3("World!"),
        test_line(""),
        check_value(false) {
  }
  void add_one_element_to_queue();
  void extract_from_queue();
  void add_three_elements_to_queue();
  void ShutDownQueue();

  static void* add_one_element_to_queue_helper(void *context);
  static void* extract_from_queue_helper(void *context);
  static void* add_three_elements_to_queue_helper(void *context);
  static void* ShutDownQueue_helper(void *context);

 protected:
  MessageQueue<std::string> test_queue;
  std::string test_val_1;
  std::string test_val_2;
  std::string test_val_3;
  std::string test_line;
  bool check_value;
};

// Thread function - adds 1 element1 to the queue
void MessageQueueTest::add_one_element_to_queue() {
  test_queue.push(test_val_1);
  pthread_exit(NULL);
}

// Thread function - removes 1 element from beginning of queue
void MessageQueueTest::extract_from_queue() {
  test_queue.wait();
  test_queue.pop(test_line);
  pthread_exit(NULL);
}

// Thread function - adds 3 elements to the queue
void MessageQueueTest::add_three_elements_to_queue() {
  test_queue.push(test_val_1);
  test_queue.push(test_val_2);
  test_queue.push(test_val_3);
  pthread_exit(NULL);
}

// Thread function - adds 3 elements to the queue
void MessageQueueTest::ShutDownQueue() {
  check_value = true;
  test_queue.Shutdown();
  pthread_exit(NULL);
}

void* MessageQueueTest::add_one_element_to_queue_helper(void *context) {
  (reinterpret_cast<MessageQueueTest *>(context))->add_one_element_to_queue();
  return NULL;
}
void* MessageQueueTest::extract_from_queue_helper(void *context) {
  (reinterpret_cast<MessageQueueTest *>(context))->extract_from_queue();
  return NULL;
}
void* MessageQueueTest::add_three_elements_to_queue_helper(void *context) {
  (reinterpret_cast<MessageQueueTest *>(context))->add_three_elements_to_queue();
  return NULL;
}
void* MessageQueueTest::ShutDownQueue_helper(void *context) {
  (reinterpret_cast<MessageQueueTest *>(context))->ShutDownQueue();
  return NULL;
}

TEST_F(MessageQueueTest, DefaultCtorTest_ExpectEmptyQueueCreated) {
  bool test_value = true;
  // Check if the queue is empty
  ASSERT_EQ(test_value, test_queue.empty());
}

TEST_F(MessageQueueTest, MessageQueuePushThreeElementsTest_ExpectThreeElementsAdded) {
  pthread_t thread1;
  pthread_create(&thread1, NULL, &MessageQueueTest::add_three_elements_to_queue_helper, this);
  pthread_join(thread1, NULL);
  // check if 3 elements were added successfully
  ASSERT_EQ(3u, test_queue.size());
}

TEST_F(MessageQueueTest, NotEmptyMessageQueueResetTest_ExpectEmptyQueue) {
  // Adding some elements to queue
  test_queue.push(test_val_1);
  test_queue.push(test_val_2);
  test_queue.push(test_val_3);
  // Resetting queue
  test_queue.Reset();
  // Check if queue is empty
  ASSERT_TRUE(test_queue.empty());
  // Check the size of queue after reset
  ASSERT_EQ(0u, test_queue.size());
}

TEST_F(MessageQueueTest, MessageQueuePopOneElementTest_ExpectOneElementRemovedFromQueue) {
  pthread_t thread1;
  pthread_t thread2;
  // Creating threads with thread function mentioned above
  pthread_create(&thread1, NULL, &MessageQueueTest::add_one_element_to_queue_helper, this);
  pthread_create(&thread2, NULL, &MessageQueueTest::extract_from_queue_helper, this);
  // Primary thread waits until thread 2 to be finished
  pthread_join(thread2, NULL);
  // Check if first element was removed successfully
  ASSERT_EQ(test_val_1, test_line);
  // Check the size of queue after 1 element was removed
  ASSERT_EQ(0u, test_queue.size());
}

TEST_F(MessageQueueTest, MessageQueueShutdownTest_ExpectMessageQueueWillBeShutDown) {
  pthread_t thread1;
  // Creating thread with thread function mentioned above
  pthread_create(&thread1, NULL, &MessageQueueTest::ShutDownQueue_helper, this);
  // Primary thread sleeps until thread1 will make queue shutdown
  test_queue.wait();
  check_value = true;
  ASSERT_TRUE(check_value);
}

}  // namespace utils
}  // namespace components
}  // namespace test
