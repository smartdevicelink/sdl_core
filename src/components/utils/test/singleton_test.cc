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

#include "gtest/gtest.h"
#include "utils/singleton.h"
#if defined(OS_POSIX)
#include <pthread.h>
#endif
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace test {
namespace components {
namespace utils_test {

using ::utils::Singleton;

class SingletonTest : public ::utils::Singleton<SingletonTest> {
 public:
  void SetValue(int value) {
    test_value = value;
  }
  int GetValue() {
    return test_value;
  }

  FRIEND_BASE_SINGLETON_CLASS(SingletonTest);

 private:
  int test_value;
};

TEST(SingletonTest, CreateAndDestroySingleton) {
  // assert
  ASSERT_EQ(SingletonTest::instance(), SingletonTest::instance());
  ASSERT_EQ(0, SingletonTest::instance()->GetValue());
  ASSERT_TRUE(SingletonTest::exists());
  SingletonTest::instance()->SetValue(5);
  ASSERT_EQ(5, SingletonTest::instance()->GetValue());

  // act
  SingletonTest::destroy();

  // assert
  ASSERT_FALSE(SingletonTest::exists());
}

TEST(SingletonTest, DestroySingletonTwice) {
  // assert
  ASSERT_EQ(0, SingletonTest::instance()->GetValue());
  ASSERT_TRUE(SingletonTest::exists());

  // act
  SingletonTest::destroy();
  // assert
  ASSERT_FALSE(SingletonTest::exists());

  // act
  SingletonTest::destroy();
  // assert
  ASSERT_FALSE(SingletonTest::exists());
}

TEST(SingletonTest, DeleteSingletonCreateAnother) {
  // arrange
  SingletonTest::instance()->SetValue(10);
  // assert
  ASSERT_TRUE(SingletonTest::exists());
  ASSERT_EQ(10, SingletonTest::instance()->GetValue());
  // act
  SingletonTest::destroy();
  // assert
  ASSERT_FALSE(SingletonTest::exists());

  // act
  SingletonTest::instance();

  // assert
  ASSERT_EQ(0, SingletonTest::instance()->GetValue());
  ASSERT_TRUE(SingletonTest::exists());
  SingletonTest::destroy();
}

class CreateSingletonDelegate : public threads::ThreadDelegate {
 public:
  void threadMain() {
    SingletonTest::instance();
  }
  void exitThreadMain() {}
};

class DestroySingletonDelegate : public threads::ThreadDelegate {
 public:
  DestroySingletonDelegate() {}
  DestroySingletonDelegate(void* value) : value_(value) {}
  void threadMain() {
    SingletonTest* instance = reinterpret_cast<SingletonTest*>(value_);
    instance->destroy();
  }
  void exitThreadMain() {}

 private:
  void* value_;
};

TEST(SingletonTest, CreateSingletonInDifferentThreads) {
  // arrange
  SingletonTest::instance();
  ASSERT_TRUE(SingletonTest::exists());

  threads::Thread* thread1 =
      threads::CreateThread("test thread", new CreateSingletonDelegate());
  threads::Thread* thread2 =
      threads::CreateThread("test thread", new CreateSingletonDelegate());
  thread1->start();
  thread2->start();
  thread1->join();
  thread2->join();
  threads::DeleteThread(thread1);
  threads::DeleteThread(thread2);

  // act
  SingletonTest::destroy();
  // assert
  ASSERT_FALSE(SingletonTest::exists());
}

#if defined(OS_POSIX)
void* func_pthread1(void*) {
  SingletonTest* singleton_in_other_thread = SingletonTest::instance();
  pthread_exit(singleton_in_other_thread);
  return NULL;
}

TEST(SingletonTest, CreateDeleteSingletonInDifferentThreads) {
  // arrange
  pthread_t thread1;
  pthread_create(&thread1, NULL, func_pthread1, NULL);

  pthread_t thread2;
  pthread_create(&thread2, NULL, func_pthread1, NULL);

  void* instance1;
  pthread_join(thread1, &instance1);
  SingletonTest* instance_1 = reinterpret_cast<SingletonTest*>(instance1);

  void* instance2;
  pthread_join(thread2, &instance2);
  SingletonTest* instance_2 = reinterpret_cast<SingletonTest*>(instance2);

  // assert
  ASSERT_TRUE(instance_1->exists());
  ASSERT_TRUE(instance_2->exists());

  ASSERT_EQ(instance_1, instance_2);

  // act
  SingletonTest::destroy();

  // assert
  ASSERT_FALSE(instance_1->exists());
  ASSERT_FALSE(instance_2->exists());
}
#endif

TEST(SingletonTest, DISABLED_DeleteSingletonInDifferentThread) {
  // arrange
  SingletonTest::instance();
  ASSERT_TRUE(SingletonTest::exists());

  threads::Thread* thread1 = threads::CreateThread(
      "test thread", new DestroySingletonDelegate(SingletonTest::instance()));
  thread1->start();
  thread1->join();

  // assert
  ASSERT_FALSE(SingletonTest::exists());
  threads::DeleteThread(thread1);
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
