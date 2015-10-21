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

#include <vector>
#include <map>
#include "gtest/gtest.h"
#include "utils/stl_utils.h"

namespace test {
namespace components {
namespace utils {

using ::utils::StlCollectionDeleter;
using ::utils::StlMapDeleter;

class TestObject {
 public:
  ~TestObject() {
  }
};

typedef std::map<int, TestObject*> TestMap;
typedef std::vector<TestObject*> TestVector;

TEST(StlDeleter, DestructMapWithOneElement) {
  TestMap test_map;
  test_map[1] = new TestObject();

  EXPECT_EQ(1u, test_map.size());
  {
    StlMapDeleter<TestMap> test_list_deleter_(&test_map);
  }
  EXPECT_EQ(1u, test_map.size());
  EXPECT_EQ(NULL, test_map[1]);
}

TEST(StlDeleter, DestructMapWithSeveralElements) {
  TestMap test_map;
  test_map[1] = new TestObject();
  test_map[2] = new TestObject();

  EXPECT_EQ(2u, test_map.size());
  {
    StlMapDeleter<TestMap> test_list_deleter_(&test_map);
  }
  EXPECT_EQ(2u, test_map.size());
  EXPECT_EQ(NULL, test_map[1]);
  EXPECT_EQ(NULL, test_map[2]);
}

TEST(StlDeleter, DestructVectorWithOneElement) {
  TestVector test_vector;
  test_vector.push_back(new TestObject());

  EXPECT_EQ(1u, test_vector.size());
  {
    StlCollectionDeleter<TestVector> test_list_deleter_(&test_vector);
  }
  EXPECT_EQ(1u, test_vector.size());
  EXPECT_EQ(NULL, test_vector[0]);
}

TEST(StlDeleter, DestructVectorWithSeveralElements) {
  TestVector test_vector;
  test_vector.push_back(new TestObject());
  test_vector.push_back(new TestObject());

  EXPECT_EQ(2u, test_vector.size());
  {
    StlCollectionDeleter<TestVector> test_list_deleter_(&test_vector);
  }
  EXPECT_EQ(2u, test_vector.size());
  EXPECT_EQ(NULL, test_vector[0]);
  EXPECT_EQ(NULL, test_vector[1]);
}

}  // namespace utils
}  // namespace components
}  // namespace test
