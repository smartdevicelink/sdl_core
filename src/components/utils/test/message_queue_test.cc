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
#include "utils/message_queue.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace test {
namespace components {
namespace utils_test {

using ::utils::MessageQueue;

class MessageQueueTest : public testing::Test {
 public:
  MessageQueueTest() {}
  static void add_one_element_to_queue();
  static void extract_from_queue();
  static void add_three_elements_to_queue();
  static void ShutDownQueue();

 protected:
  static MessageQueue<std::string> test_queue;
  static std::string test_val_1;
  static std::string test_val_2;
  static std::string test_val_3;
  static std::string test_line;
  static bool check_value;
};

MessageQueue<std::string> MessageQueueTest::test_queue;
std::string MessageQueueTest::test_val_1 = "Hello,";
std::string MessageQueueTest::test_val_2 = "Beautiful ";
std::string MessageQueueTest::test_val_3 = "World!";
std::string MessageQueueTest::test_line = "";
bool MessageQueueTest::check_value = false;

// Thread function - adds 1 element1 to the queue
void MessageQueueTest::add_one_element_to_queue() {
  test_queue.push(test_val_1);
}

// Thread function - removes 1 element from beginning of queue
void MessageQueueTest::extract_from_queue() {
  test_queue.wait();
  test_queue.pop(test_line);
}

// Thread function - adds 3 elements to the queue
void MessageQueueTest::ShutDownQueue() {
  check_value = true;
  test_queue.Shutdown();
}

class AddThreeElementsDelegate : public threads::ThreadDelegate {
 public:
  AddThreeElementsDelegate(MessageQueue<std::string>& queue)
      : test_queue_(queue) {}

  void threadMain() {
    test_queue_.push("test_val_1");
    test_queue_.push("test_val_2");
    test_queue_.push("test_val_3");
  }
  void exitThreadMain() {}

 private:
  MessageQueue<std::string>& test_queue_;
};

class AddOneElementDelegate : public threads::ThreadDelegate {
 public:
  void threadMain() {
    MessageQueueTest::add_one_element_to_queue();
  }
  void exitThreadMain() {}
};

class ExtractFromQueueDelegate : public threads::ThreadDelegate {
 public:
  void threadMain() {
    MessageQueueTest::extract_from_queue();
  }
  void exitThreadMain() {}
};

class ShutDownQueueDelegate : public threads::ThreadDelegate {
 public:
  void threadMain() {
    MessageQueueTest::ShutDownQueue();
  }
  void exitThreadMain() {}
};

TEST_F(MessageQueueTest, DefaultCtorTest_ExpectEmptyQueueCreated) {
  bool test_value = true;
  // Check if the queue is empty
  ASSERT_EQ(test_value, test_queue.empty());
}

TEST_F(MessageQueueTest,
       MessageQueuePushThreeElementsTest_ExpectThreeElementsAdded) {
  MessageQueue<std::string> test_queue1;
  threads::Thread* thread = threads::CreateThread(
      "test thread", new AddThreeElementsDelegate(test_queue1));
  thread->start();
  thread->join(threads::Thread::kNoStop);
  ASSERT_EQ(3u, test_queue1.size());
  threads::DeleteThread(thread);
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

TEST_F(MessageQueueTest,
       MessageQueuePopOneElementTest_ExpectOneElementRemovedFromQueue) {
  threads::Thread* thread1 =
      threads::CreateThread("test thread", new AddOneElementDelegate());
  threads::Thread* thread2 =
      threads::CreateThread("test thread", new ExtractFromQueueDelegate());
  thread1->start();
  thread2->start();
  thread1->join(threads::Thread::kNoStop);
  thread2->join(threads::Thread::kNoStop);

  // Check if first element was removed successfully
  ASSERT_EQ(test_val_1, test_line);
  // Check the size of queue after 1 element was removed
  ASSERT_EQ(0u, test_queue.size());
  threads::DeleteThread(thread1);
  threads::DeleteThread(thread2);
}

TEST_F(MessageQueueTest,
       MessageQueueShutdownTest_ExpectMessageQueueWillBeShutDown) {
  threads::Thread* thread1 =
      threads::CreateThread("test thread", new ShutDownQueueDelegate());
  thread1->start();
  thread1->join(threads::Thread::kNoStop);

  // Primary thread sleeps until thread1 will make queue shutdown
  test_queue.wait();
  check_value = true;
  ASSERT_TRUE(check_value);
  threads::DeleteThread(thread1);
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
