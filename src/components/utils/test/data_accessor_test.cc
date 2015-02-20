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
#include "utils/data_accessor.h"
#include "utils/lock.h"

namespace test {
namespace components {
namespace utils {

TEST(DataAccessorTest, CreateDataAccessor) {

  //arrange
  int test_value = 10;
  sync_primitives::Lock testSet_lock_;
  DataAccessor<int> testdata(test_value, testSet_lock_);
  int data_from_testdata = testdata.GetData();

  //assert
  EXPECT_EQ(test_value, data_from_testdata);
}

TEST(DataAccessorTest, CreateDataAccessor_MutexIsLocked_CannotLockItAgain) {

  //arrange
  int test_value = 10;
  sync_primitives::Lock testSet_lock_;
  DataAccessor<int> testdata(test_value, testSet_lock_);

  //assert
  EXPECT_FALSE(testSet_lock_.Try());
}

TEST(DataAccessorTest, CopyDataAccessor_GetDataFromDataAccessors) {

  //arrange
  int test_value = 10;
  sync_primitives::Lock testSet_lock_;
  DataAccessor<int> testdata(test_value, testSet_lock_);
  DataAccessor<int> testdata_copy(testdata);

  int data_from_testdata = testdata.GetData();
  int data_from_testdata_copy = testdata_copy.GetData();

  //assert
  EXPECT_EQ(data_from_testdata, data_from_testdata_copy);

  EXPECT_FALSE(testSet_lock_.Try());
}

TEST(DataAccessorTest,ChangedDataInDataAccessor_ChangeData_DataInDataAccessorIsChanged) {

  //arrange
  int test_value = 10;
  sync_primitives::Lock testSet_lock_;
  DataAccessor<int> testdata(test_value, testSet_lock_);
  test_value = 0;

  int data_from_testdata_after_change = testdata.GetData();

  //assert
  EXPECT_EQ(test_value, data_from_testdata_after_change);
}

TEST(DataAccessorTest, DeleteDataAccessor_CreatedOneDeleteOneThread_MutexIsUnlocked) {

  //arrange
  int test_value = 10;
  sync_primitives::Lock testSet_lock_;
  {
    DataAccessor<int> testdata(test_value, testSet_lock_);

    //assert
    EXPECT_FALSE(testSet_lock_.Try());
  }
  //assert

  EXPECT_TRUE(testSet_lock_.Try());

  testSet_lock_.Release();

}

TEST(DataAccessorTest, DeleteDataAccessor_CreatedThreadAndCopyDeleteBothThreads_MutexIsUnlocked) {

  //arrange
  int test_value = 10;
  sync_primitives::Lock testSet_lock_;
  {
    DataAccessor<int> testdata(test_value, testSet_lock_);
    {
      DataAccessor<int> testdata_copy(testdata);

      //assert
      EXPECT_FALSE(testSet_lock_.Try());
    }
    //assert
    EXPECT_FALSE(testSet_lock_.Try());

  }

  //assert
  EXPECT_TRUE(testSet_lock_.Try());
  testSet_lock_.Release();

}

}  // namespace utils
}  // namespace components
}  // namespace test
