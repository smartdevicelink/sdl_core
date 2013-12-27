#ifndef TEST_COMPONENTS_UTILS_INCLUDE_UTILS_PRIORITIZED_QUEUE_H_
#define TEST_COMPONENTS_UTILS_INCLUDE_UTILS_PRIORITIZED_QUEUE_H_

#include "utils/prioritized_queue.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace test  {
namespace components  {
namespace utils  {
  TEST(PrioritizedQueueTest, CommonTests) {
    ASSERT_TRUE(true);
  struct TestMessage {
    int order;
    int priority;
    int PriorityOrder() const { return priority; }
  };

  const size_t kSequenceLength = 8;
  TestMessage input[kSequenceLength] = {
      {0, 0}, {1, 0}, {2, 2}, {3, 0}, {4, 2}, {5, 0}, {6, 9}, {7, 1}
  };
  TestMessage output[kSequenceLength] = {
      {6, 9}, {2, 2}, {4, 2}, {7, 1}, {0, 0}, {1, 0}, {3, 0}, {5, 0}
  };
  TestMessage reversed_output[kSequenceLength] = {
      {6, 9}, {4, 2}, {2, 2}, {7, 1}, {5, 0}, {3, 0}, {1, 0}, {0, 0}
  };

  ::utils::PrioritizedQueue<TestMessage> queue;
  // Push in direct order
  for (size_t i = 0; i < kSequenceLength; ++i) {
    queue.push(input[i]);
  }
  ASSERT_TRUE(queue.size() == kSequenceLength);
  for (size_t i = 0; i < kSequenceLength; ++i) {
    ASSERT_TRUE(!queue.empty());
    ASSERT_TRUE(queue.size() == kSequenceLength - i);
    ASSERT_TRUE(queue.front().order == output[i].order);
    queue.pop();
  }
  ASSERT_TRUE(queue.empty());

  // Push in reverse order
  for (int i = kSequenceLength - 1; i >= 0 ; --i) {
    queue.push(input[i]);
  }
  ASSERT_TRUE(queue.size() == kSequenceLength);
  for (size_t i = 0; i < kSequenceLength; ++i) {
    ASSERT_TRUE(!queue.empty());
    ASSERT_TRUE(queue.size() == kSequenceLength - i);
    ASSERT_TRUE(queue.front().order == reversed_output[i].order);
    queue.pop();
  }
}
}  // namespace utils
}  // namespace components
}  // namespace test

#endif // TEST_COMPONENTS_UTILS_INCLUDE_UTILS_PRIORITIZED_QUEUE_H_
