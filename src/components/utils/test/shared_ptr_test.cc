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

#include <stdlib.h>
#include <vector>
#include <iostream>
#include "gmock/gmock.h"
#include "utils/shared_ptr.h"

namespace test {
namespace components {
namespace utils {
namespace SharedPtrTest {

class CMockObject {
  public:
    CMockObject(int id);
    virtual ~CMockObject();
    virtual int getId() const;

    MOCK_METHOD0(destructor, void ());

  private:
    int mId_;
};

class CExtendedMockObject : public CMockObject {
  public:
    CExtendedMockObject(int id);
};

}  // namespace CMockObject
}  // namespace SmartObjects
}  // namespace components
}  // namespace test

using namespace test::components::utils::SharedPtrTest;
using ::testing::NiceMock;

CMockObject::CMockObject(int id)
    : mId_(id) {
}

CMockObject::~CMockObject() {
  destructor();
}

int CMockObject::getId() const {
  return mId_;
}

CExtendedMockObject::CExtendedMockObject(int id)
    : CMockObject(id) {
}

typedef utils::SharedPtr<CMockObject> tMockObjectPtr;
typedef utils::SharedPtr<CExtendedMockObject> tExtendedMockObjectPtr;

TEST(SharedPtrTest, DefaultConstructorTest) {
  // Constructor checks
  tMockObjectPtr p0;
  ASSERT_EQ(0, p0.get());
  ASSERT_FALSE(p0.valid());
}

TEST(SharedPtrTest, ConstructorWithOneParameterTest) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  EXPECT_CALL(*object1, destructor()).Times(1);

  // Constructor checks
  tMockObjectPtr p1(object1);
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(1u, *(p1.get_ReferenceCounter()));
}

TEST(SharedPtrTest, CopyConstructorTest) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  EXPECT_CALL(*object1, destructor()).Times(1);

  // Constructor checks
  tMockObjectPtr p1(object1);
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(1u, *(p1.get_ReferenceCounter()));

  tMockObjectPtr p2(p1);
  ASSERT_EQ(1, p2->getId());
  ASSERT_EQ(2u, *(p2.get_ReferenceCounter()));
  tMockObjectPtr p3 = p2;
  ASSERT_EQ(1, p3->getId());
  ASSERT_EQ(3u, *(p3.get_ReferenceCounter()));
  {
  tMockObjectPtr p4 = p3;
  ASSERT_EQ(1, p4->getId());
  ASSERT_EQ(4u, *(p3.get_ReferenceCounter()));
  }
  // Check reference counter decreased
  ASSERT_EQ(3u, *(p3.get_ReferenceCounter()));
}

TEST(SharedPtrTest, SecondConstructorWithOneParameterTest) {
  // Arrange
  CExtendedMockObject* object1 = new CExtendedMockObject(2);
  EXPECT_CALL(*object1, destructor()).Times(0);

  // Constructors checks
  tExtendedMockObjectPtr p1(object1);
  ASSERT_EQ(2, p1->getId());
  ASSERT_EQ(1u, *(p1.get_ReferenceCounter()));

  tMockObjectPtr p2(p1);
  ASSERT_EQ(2, p2->getId());
  ASSERT_EQ(2u, *(p2.get_ReferenceCounter()));
  EXPECT_CALL(*object1, destructor());
}

TEST(SharedPtrTest, AssignmentOperatorTest) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  CMockObject* object2 = new CMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object2, destructor()).Times(0);

  tMockObjectPtr p1(object1);
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(1u, *(p1.get_ReferenceCounter()));

  tMockObjectPtr p2(object2);
  ASSERT_EQ(2, p2->getId());
  ASSERT_EQ(1u, *(p2.get_ReferenceCounter()));

  tMockObjectPtr p3(p1);
  ASSERT_EQ(1, p3->getId());
  ASSERT_EQ(2u, *(p3.get_ReferenceCounter()));

  tMockObjectPtr p4(p3);
  ASSERT_EQ(1, p4->getId());
  ASSERT_EQ(3u, *(p4.get_ReferenceCounter()));

  tMockObjectPtr p5(p4);
  ASSERT_EQ(1, p5->getId());
  ASSERT_EQ(4u, *(p5.get_ReferenceCounter()));

  p5 = p2;

  // Check reference counter for new SharedPtr increased
  ASSERT_EQ(2, p5->getId());
  ASSERT_EQ(2u, *(p5.get_ReferenceCounter()));

  // Check reference counter for old SharedPtr decreased
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(3u, *(p1.get_ReferenceCounter()));

  EXPECT_CALL(*object1, destructor());
  EXPECT_CALL(*object2, destructor());
}

TEST(SharedPtrTest, SecondAssignmentOperatorTest) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  CExtendedMockObject* object2 = new CExtendedMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object2, destructor()).Times(0);

  tMockObjectPtr p1(object1);
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(1u, *(p1.get_ReferenceCounter()));

  tExtendedMockObjectPtr p2(object2);
  ASSERT_EQ(2, p2->getId());
  ASSERT_EQ(1u, *(p2.get_ReferenceCounter()));

  tMockObjectPtr p3(p1);
  ASSERT_EQ(1, p3->getId());
  ASSERT_EQ(2u, *(p3.get_ReferenceCounter()));

  tMockObjectPtr p4(p3);
  ASSERT_EQ(1, p4->getId());
  ASSERT_EQ(3u, *(p4.get_ReferenceCounter()));

  tMockObjectPtr p5(p4);
  ASSERT_EQ(1, p5->getId());
  ASSERT_EQ(4u, *(p5.get_ReferenceCounter()));
  // Use assignment operator
  p5 = p2;

  // Check reference counter for new SharedPtr increased
  ASSERT_EQ(2, p5->getId());
  ASSERT_EQ(2u, *(p5.get_ReferenceCounter()));

  // Check reference counter for old SharedPtr decreased
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(3u, *(p1.get_ReferenceCounter()));

  EXPECT_CALL(*object1, destructor());
  EXPECT_CALL(*object2, destructor());
}

TEST(SharedPtrTest, EqualOperatorTest) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  CMockObject* object2 = new CMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object2, destructor()).Times(0);

  tMockObjectPtr p1(object1);
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(1u, *(p1.get_ReferenceCounter()));

  tMockObjectPtr p2(object2);
  ASSERT_EQ(2, p2->getId());
  ASSERT_EQ(1u, *(p2.get_ReferenceCounter()));

  tMockObjectPtr p3(p1);
  ASSERT_EQ(1, p3->getId());
  ASSERT_EQ(2u, *(p3.get_ReferenceCounter()));

  tMockObjectPtr p4(p3);
  ASSERT_EQ(1, p4->getId());
  ASSERT_EQ(3u, *(p4.get_ReferenceCounter()));

  tMockObjectPtr p5(p4);
  ASSERT_EQ(1, p5->getId());
  ASSERT_EQ(4u, *(p5.get_ReferenceCounter()));
  // Checks
  ASSERT_TRUE(p1 == p3);
  ASSERT_TRUE(p1 == p4);
  ASSERT_TRUE(p4 == p3);
  ASSERT_FALSE(p1 == p2);

  EXPECT_CALL(*object1, destructor());
  EXPECT_CALL(*object2, destructor());
}

TEST(SharedPtrTest, LessThanOperatorTest) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  CMockObject* object2 = new CMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object2, destructor()).Times(0);

  tMockObjectPtr p1(object1);
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(1u, *(p1.get_ReferenceCounter()));

  tMockObjectPtr p2(object2);
  ASSERT_EQ(2, p2->getId());
  ASSERT_EQ(1u, *(p2.get_ReferenceCounter()));

  // Checks
  if (object1 < object2) {
      ASSERT_TRUE(p1 < p2);
  }
  else {
  ASSERT_FALSE(p1 < p2);
  }

  EXPECT_CALL(*object1, destructor());
  EXPECT_CALL(*object2, destructor());
}

TEST(SharedPtrTest, StaticPointerCastTest_DerivedToBase_ExpectCastOk) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  CExtendedMockObject* object2 = new CExtendedMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(1);
  EXPECT_CALL(*object2, destructor()).Times(0);

  tMockObjectPtr p1(object1);
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(1u, *(p1.get_ReferenceCounter()));

  tExtendedMockObjectPtr ep1(object2);
  ASSERT_EQ(2, ep1->getId());
  ASSERT_EQ(1u, *(ep1.get_ReferenceCounter()));
  // Cast from SharedPtr to Derived class to SharedPtr to Base class
  p1 = utils::SharedPtr<CExtendedMockObject>::static_pointer_cast< CMockObject >(ep1);
  // Checks
  ASSERT_EQ(2, p1->getId());
  ASSERT_EQ(2u, *(p1.get_ReferenceCounter()));
  ASSERT_TRUE(p1 == ep1);

  EXPECT_CALL(*object2, destructor());
}

TEST(SharedPtrTest, StaticPointerCastTest_BaseToDerived_ExpectCastOk) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  CExtendedMockObject* object2 = new CExtendedMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object2, destructor()).Times(1);

  tMockObjectPtr p1(object1);
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(1u, *(p1.get_ReferenceCounter()));

  tExtendedMockObjectPtr ep1(object2);
  ASSERT_EQ(2, ep1->getId());
  ASSERT_EQ(1u, *(ep1.get_ReferenceCounter()));
  // Cast from SharedPtr to Base class to SharedPtr to Derived class
  ep1 = utils::SharedPtr<CMockObject>::static_pointer_cast<CExtendedMockObject>(p1);
  // Checks
  ASSERT_EQ(1, ep1->getId());
  ASSERT_EQ(2u, *(ep1.get_ReferenceCounter()));
  ASSERT_TRUE(p1 == ep1);

  EXPECT_CALL(*object1, destructor());
}

TEST(SharedPtrTest, DynamicPointerCastTest_DerivedToBase_ExpectCastOk) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  CExtendedMockObject* object2 = new CExtendedMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(1);
  EXPECT_CALL(*object2, destructor()).Times(0);

  tMockObjectPtr p1(object1);
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(1u, *(p1.get_ReferenceCounter()));

  tExtendedMockObjectPtr ep1(object2);
  ASSERT_EQ(2, ep1->getId());
  ASSERT_EQ(1u, *(ep1.get_ReferenceCounter()));
  // Cast from SharedPtr to Derived class to SharedPtr to Base class
  p1 = utils::SharedPtr<CExtendedMockObject>::dynamic_pointer_cast< CMockObject >(ep1);
  // Checks
  ASSERT_EQ(2, p1->getId());
  ASSERT_EQ(2u, *(p1.get_ReferenceCounter()));
  ASSERT_TRUE(p1 == ep1);

  EXPECT_CALL(*object2, destructor());
}

TEST(SharedPtrTest, DynamicPointerCastTest_BaseToDerived_ExpectNullPtr) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  CExtendedMockObject* object2 = new CExtendedMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object2, destructor()).Times(1);

  tMockObjectPtr p1(object1);
  ASSERT_EQ(1, p1->getId());
  ASSERT_EQ(1u, *(p1.get_ReferenceCounter()));

  tExtendedMockObjectPtr ep1(object2);
  ASSERT_EQ(2, ep1->getId());
  ASSERT_EQ(1u, *(ep1.get_ReferenceCounter()));
  // Cast from SharedPtr to Base class to SharedPtr to Derived class
  ep1 = utils::SharedPtr<CMockObject>::dynamic_pointer_cast<CExtendedMockObject>(p1);
  // Checks
  ASSERT_EQ(NULL, ep1);

  EXPECT_CALL(*object1, destructor());
}

TEST(SharedPtrTest, ArrowOperatorTest) {
  // Arrange
  CExtendedMockObject* object1 = new CExtendedMockObject(1);
  CExtendedMockObject* object2 = new CExtendedMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(1);
  EXPECT_CALL(*object2, destructor()).Times(1);

  tExtendedMockObjectPtr ep1(object1);
  // Check
  ASSERT_EQ(1, ep1->getId());

  tMockObjectPtr p1(ep1);
  // Check
  ASSERT_EQ(1, p1->getId());

  tExtendedMockObjectPtr ep2(object2);
  // Check
  ASSERT_EQ(2, ep2->getId());
}

TEST(SharedPtrTest, DereferenceOperatorTest) {
  // Arrange
  CExtendedMockObject* object1 = new CExtendedMockObject(1);
  CExtendedMockObject* object2 = new CExtendedMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(1);
  EXPECT_CALL(*object2, destructor()).Times(1);

  tExtendedMockObjectPtr ep1(object1);
  // Check
  ASSERT_EQ(1, (*ep1).getId());

  tMockObjectPtr p1(ep1);
  // Check
  ASSERT_EQ(1, (*p1).getId());

  tExtendedMockObjectPtr ep2(object2);
  // Check
  ASSERT_EQ(2, (*ep2).getId());
}

TEST(SharedPtrTest, BoolOperatorTest) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  tMockObjectPtr p1(object1);
  tMockObjectPtr p2;

  // Checks
  ASSERT_TRUE(p1);
  ASSERT_FALSE(p2);
  EXPECT_CALL(*object1, destructor());
}

TEST(SharedPtrTest, ResetWithoutArgsTest) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  CMockObject* object2 = new CMockObject(2);

  EXPECT_CALL(*object1, destructor()).Times(1);
  EXPECT_CALL(*object2, destructor()).Times(1);

  tMockObjectPtr p1(object1);
  tMockObjectPtr p2(object2);
  ASSERT_EQ(2, p2->getId());
  ASSERT_EQ(1u, *(p2.get_ReferenceCounter()));

  tMockObjectPtr p3(p1);
  tMockObjectPtr p4(p3);
  ASSERT_EQ(1, p4->getId());
  ASSERT_EQ(3u, *(p4.get_ReferenceCounter()));
  // Act
  p2.reset();
  // Check
  EXPECT_EQ(NULL, p2.get());
  EXPECT_EQ(1u, *(p2.get_ReferenceCounter()));
  p4.reset();
  // Check
  EXPECT_EQ(NULL, p4.get());
  EXPECT_EQ(1u, *(p4.get_ReferenceCounter()));
}

TEST(SharedPtrTest, ResetWithArgumentTest) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  CMockObject* object2 = new CMockObject(27);

  EXPECT_CALL(*object1, destructor()).Times(1);
  EXPECT_CALL(*object2, destructor()).Times(1);

  tMockObjectPtr p1(object1);
  tMockObjectPtr p3(p1);
  tMockObjectPtr p4(p3);
  ASSERT_EQ(1, p4->getId());
  ASSERT_EQ(3u, *(p4.get_ReferenceCounter()));
  // Act
  p4.reset(object2);
  // Check
  EXPECT_EQ(27, (*p4).getId());
  EXPECT_EQ(1u, *(p4.get_ReferenceCounter()));
}

TEST(SharedPtrTest, GetMethodTest_ExpectObjPointer) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  EXPECT_CALL(*object1, destructor()).Times(1);
  tMockObjectPtr p1(object1);
  // Check
  ASSERT_EQ(object1, p1.get());
}

TEST(SharedPtrTest, ValidMethodTest_ExpectCorrectValidation) {
  // Arrange
  CMockObject* object1 = new CMockObject(1);
  EXPECT_CALL(*object1, destructor()).Times(1);

  tMockObjectPtr p1(object1);
  tMockObjectPtr p2;
  // Check
  ASSERT_TRUE(p1.valid());
  ASSERT_FALSE(p2.valid());
}

TEST(SharedPtrTest, StressTest) {
  // Arrange
  const size_t kNumIterations = 1024U * 1024U;

  size_t objectCreated = 0U;
  size_t pointersCopied = 0U;
  std::vector<tMockObjectPtr> objects;

  for (size_t i = 0U; i < kNumIterations; ++i) {
    if ((true == objects.empty()) || (0 == rand() % 256)) {
      CMockObject* object = new CMockObject(0);
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
