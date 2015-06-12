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

#include <stdlib.h>
#include <vector>
#include "gmock/gmock.h"
#include "utils/shared_ptr.h"

namespace test {
namespace components {
namespace utils {
namespace SharedPtrTest {
class CMockObject {
 public:
  CMockObject(int id);
  ~CMockObject(void);
  int getId(void) const;

  MOCK_METHOD0(destructor, void ());

private:
  int mId;
};

class CExtendedMockObject : public CMockObject {
 public:
  CExtendedMockObject(int id);
};
}  // namespace CMockObject
}  // namespace SmartObjects
}  // namespace components
}  // namespace test

test::components::utils::SharedPtrTest::CMockObject::CMockObject(int id)
    : mId(id) {
}

test::components::utils::SharedPtrTest::CMockObject::~CMockObject(void) {
  destructor();
}

int test::components::utils::SharedPtrTest::CMockObject::getId(void) const {
  return mId;
}

test::components::utils::SharedPtrTest::CExtendedMockObject::CExtendedMockObject(
    int id)
    : CMockObject(id) {
}

typedef utils::SharedPtr<test::components::utils::SharedPtrTest::CMockObject> tMockObjectPtr;
typedef utils::SharedPtr<
    test::components::utils::SharedPtrTest::CExtendedMockObject> tExtendedMockObjectPtr;

TEST(SharedPtrTest, Constructor) {
  test::components::utils::SharedPtrTest::CMockObject* object1 =
      new test::components::utils::SharedPtrTest::CMockObject(1);
  test::components::utils::SharedPtrTest::CMockObject* object2 =
      new test::components::utils::SharedPtrTest::CMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object2, destructor()).Times(0);

  tMockObjectPtr p1(object1);
  ASSERT_EQ(1, p1->getId());

  tMockObjectPtr p2(p1);
  ASSERT_EQ(1, p2->getId());

  tMockObjectPtr p3 = p2;
  ASSERT_EQ(1, p3->getId());

  tMockObjectPtr p4 = object2;
  ASSERT_EQ(2, p4->getId());

  p3 = p4;
  ASSERT_EQ(2, p3->getId());

  EXPECT_CALL(*object1, destructor());
  EXPECT_CALL(*object2, destructor());
}

TEST(SharedPtrTest, PointerTypeCast) {
  test::components::utils::SharedPtrTest::CExtendedMockObject* object1 =
      new test::components::utils::SharedPtrTest::CExtendedMockObject(1);
  test::components::utils::SharedPtrTest::CExtendedMockObject* object2 =
      new test::components::utils::SharedPtrTest::CExtendedMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object2, destructor()).Times(0);

  tExtendedMockObjectPtr ep1(object1);
  ASSERT_EQ(1, ep1->getId());

  tMockObjectPtr p1(ep1);
  ASSERT_EQ(1, p1->getId());

  tExtendedMockObjectPtr ep2(object2);
  ASSERT_EQ(2, ep2->getId());

  p1 = ep2;
  ASSERT_EQ(2, p1->getId());

  EXPECT_CALL(*object1, destructor());
  EXPECT_CALL(*object2, destructor());
}

TEST(SharedPtrTest, AddedOperators) {
  test::components::utils::SharedPtrTest::CExtendedMockObject* object1 =
      new test::components::utils::SharedPtrTest::CExtendedMockObject(1);
  test::components::utils::SharedPtrTest::CExtendedMockObject* object2 =
      new test::components::utils::SharedPtrTest::CExtendedMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object2, destructor()).Times(0);

  tExtendedMockObjectPtr ep1(object1);
  tMockObjectPtr p1(ep1);
  tExtendedMockObjectPtr ep2(object2);
  p1 = ep2;

  ASSERT_EQ(2, p1->getId());
  ASSERT_EQ(2, (*p1).getId());

  ASSERT_FALSE(!p1);

  utils::SharedPtr<int> p3(new int(10));
  ASSERT_EQ(10, *p3);
  ASSERT_FALSE(!p3);

  utils::SharedPtr<int> p2;
  ASSERT_TRUE(!p2);

  p2.reset(new int);
  ASSERT_FALSE(!p2);
  *p2 = 3;
  ASSERT_EQ(3, *p2);

  EXPECT_CALL(*object1, destructor());
  EXPECT_CALL(*object2, destructor());
}

TEST(SharedPtrTest, StressTest) {
  const size_t cNumIterations = 1024U * 1024U;

  size_t objectCreated = 0U;
  size_t pointersCopied = 0U;

  std::vector<tMockObjectPtr> objects;

  for (size_t i = 0U; i < cNumIterations; ++i) {
    if ((true == objects.empty()) || (0 == rand() % 256)) {
      test::components::utils::SharedPtrTest::CMockObject* object =
          new test::components::utils::SharedPtrTest::CMockObject(0);
      EXPECT_CALL(*object, destructor());

      objects.push_back(object);

      ++objectCreated;
    } else {
      size_t objectIndex = static_cast<size_t>(rand()) % objects.size();

      if (rand() % 2) {
        objects.push_back(objects[objectIndex]);

        ++pointersCopied;
      } else {
        objects.erase(objects.begin() + objectIndex);
      }
    }
  }
  printf("%zu objects created, %zu pointers copied\n", objectCreated,
         pointersCopied);
}
