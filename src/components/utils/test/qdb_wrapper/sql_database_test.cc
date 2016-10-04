/* Copyright (c) 2013, Ford Motor Company
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

#include <cstdlib>

#include "gtest/gtest.h"

#include "qdb_wrapper/sql_error.h"
#include "qdb_wrapper/sql_database.h"

using ::utils::dbms::SQLError;
using ::utils::dbms::SQLDatabase;

namespace test {
namespace components {
namespace utils {
namespace dbms {

::testing::AssertionResult IsError(SQLError error) {
  if (error.number() != ::utils::dbms::OK) {
    return ::testing::AssertionSuccess() << error.text();
  } else {
    return ::testing::AssertionFailure() << error.text();
  }
}

TEST(SQLDatabaseTest, OpenClose) {
  SQLDatabase db("test-database");
  bool ret = db.Open();
  EXPECT_FALSE(IsError(db.LastError()));
  ASSERT_TRUE(ret);

  db.Close();
  EXPECT_FALSE(IsError(db.LastError()));
}

TEST(SQLDatabaseTest, DoubleOpen) {
  SQLDatabase db("test-database");
  bool ret = db.Open();
  EXPECT_FALSE(IsError(db.LastError()));
  ASSERT_TRUE(ret);
  ret = db.Open();
  EXPECT_FALSE(IsError(db.LastError()));
  ASSERT_TRUE(ret);
  db.Close();
}

TEST(SQLDatabaseTest, DoubleClose) {
  SQLDatabase db("test-database");
  bool ret = db.Open();
  EXPECT_FALSE(IsError(db.LastError()));
  ASSERT_TRUE(ret);

  db.Close();
  EXPECT_FALSE(IsError(db.LastError()));
  db.Close();
  EXPECT_FALSE(IsError(db.LastError()));
}

TEST(SQLDatabaseTest, CloseWithoutOpen) {
  SQLDatabase db("test-database");
  db.Close();
  EXPECT_FALSE(IsError(db.LastError()));
}

TEST(SQLDatabaseTest, CommitTransaction) {
  SQLDatabase db("test-database");
  ASSERT_TRUE(db.Open());
  EXPECT_TRUE(db.BeginTransaction());
  EXPECT_FALSE(IsError(db.LastError()));
  EXPECT_TRUE(db.CommitTransaction());
  EXPECT_FALSE(IsError(db.LastError()));
  db.Close();
}

TEST(SQLDatabaseTest, RollbackTransaction) {
  SQLDatabase db("test-database");
  ASSERT_TRUE(db.Open());
  EXPECT_TRUE(db.BeginTransaction());
  EXPECT_FALSE(IsError(db.LastError()));
  EXPECT_TRUE(db.RollbackTransaction());
  EXPECT_FALSE(IsError(db.LastError()));
  db.Close();
}

TEST(SQLDatabaseTest, FailedCommitTransaction) {
  SQLDatabase db("test-database");
  ASSERT_TRUE(db.Open());
  EXPECT_FALSE(db.CommitTransaction());
  EXPECT_TRUE(IsError(db.LastError()));
  db.Close();
}

TEST(SQLDatabaseTest, FailedRollbackTransaction) {
  SQLDatabase db("test-database");
  ASSERT_TRUE(db.Open());
  EXPECT_FALSE(db.RollbackTransaction());
  EXPECT_TRUE(IsError(db.LastError()));
  db.Close();
}

TEST(SQLDatabaseTest, BadTransaction) {
  SQLDatabase db("test-database");
  EXPECT_FALSE(db.BeginTransaction());
  EXPECT_TRUE(IsError(db.LastError()));
}

}  // namespace dbms
}  // namespace utils
}  // namespace components
}  // namespace test
