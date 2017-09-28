/* Copyright (c) 2014, Ford Motor Company
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

#include <string>
#include <sqlite3.h>

#include "gtest/gtest.h"

#include "utils/sqlite_wrapper/sql_error.h"
#include "utils/sqlite_wrapper/sql_database.h"
#include "utils/sqlite_wrapper/sql_query.h"

using ::utils::dbms::SQLError;
using ::utils::dbms::SQLDatabase;
using ::utils::dbms::SQLQuery;

namespace test {
namespace components {
namespace utils_test {
namespace dbms_test {

class SQLQueryTest : public ::testing::Test {
 protected:
  static sqlite3* conn;
  static const std::string kDatabaseName;

  static void SetUpTestCase() {
    sqlite3_open((kDatabaseName + ".sqlite").c_str(), &conn);
    sqlite3_exec(conn,
                 "CREATE TABLE testTable (integerValue INTEGER,"
                 " doubleValue REAL, stringValue TEXT)",
                 NULL,
                 NULL,
                 NULL);
  }

  static void TearDownTestCase() {
    sqlite3_close(conn);
    remove((kDatabaseName + ".sqlite").c_str());
  }

  void SetUp() {
    sqlite3_exec(conn, "DELETE FROM testTable", NULL, NULL, NULL);
  }

  ::testing::AssertionResult IsError(SQLError error) {
    if (error.number() != ::utils::dbms::OK) {
      return ::testing::AssertionSuccess() << error.text();
    } else {
      return ::testing::AssertionFailure() << error.text();
    }
  }

  ::testing::AssertionResult IsDone(SQLError error) {
    if (error.number() == ::utils::dbms::DONE) {
      return ::testing::AssertionSuccess() << error.text();
    } else {
      return ::testing::AssertionFailure() << error.text();
    }
  }

  ::testing::AssertionResult IsRow(SQLError error) {
    if (error.number() == ::utils::dbms::ROW) {
      return ::testing::AssertionSuccess() << error.text();
    } else {
      return ::testing::AssertionFailure() << error.text();
    }
  }
};

sqlite3* SQLQueryTest::conn = 0;

// mounting /tmp as tmpfs will descrease utils unit tests runing time
const std::string SQLQueryTest::kDatabaseName = "/tmp/test-query";

TEST_F(SQLQueryTest, Query_CreateQuery_QueryInDBEqualCreated) {
  // arrange
  const std::string kSelect("SELECT * FROM testTable WHERE integerValue = ?");
  SQLDatabase db(kDatabaseName);

  // assert
  ASSERT_TRUE(db.Open());

  // act
  SQLQuery query(&db);
  query.Prepare(kSelect);

  // assert
  EXPECT_STREQ(kSelect.c_str(), query.query().c_str());
}

TEST_F(SQLQueryTest, ExecString_ExecuteQuery_ActWithoutError) {
  // arrange
  const std::string kInsert(
      "INSERT INTO testTable"
      " (integerValue, doubleValue, stringValue)"
      " VALUES(2, 3.4, 'five-пять')");
  SQLDatabase db(kDatabaseName);
  // assert
  ASSERT_TRUE(db.Open());

  // act
  SQLQuery query(&db);

  // assert
  EXPECT_TRUE(query.Exec(kInsert));
  EXPECT_FALSE(IsError(query.LastError()));
}

TEST_F(SQLQueryTest,
       Bind_BindSeveralQueries_ExpectExecutedQueriesWithoutErrors) {
  // arrange
  const std::string kInsert1("INSERT INTO testTable (integerValue) VALUES (?)");
  const std::string kInsert2("INSERT INTO testTable (doubleValue) VALUES (?)");
  const std::string kInsert3("INSERT INTO testTable (stringValue) VALUES (?)");
  const std::string kInsert4(
      "INSERT INTO testTable (integerValue, doubleValue,"
      " stringValue) VALUES (?, ?, ?)");
  const int kIntegerValue = 1;
  const double kDoubleValue = 2.3;
  const std::string kStringValue = "four";

  SQLDatabase db(kDatabaseName);

  // assert
  ASSERT_TRUE(db.Open());

  // act
  SQLQuery query1(&db);

  // assert
  EXPECT_TRUE(query1.Prepare(kInsert1));
  EXPECT_FALSE(IsError(query1.LastError()));
  query1.Bind(0, kIntegerValue);

  // assert
  EXPECT_FALSE(IsError(query1.LastError()));
  EXPECT_TRUE(query1.Exec());
  EXPECT_TRUE(IsDone(query1.LastError()));

  // act
  SQLQuery query2(&db);
  // assert
  EXPECT_TRUE(query2.Prepare(kInsert2));
  EXPECT_FALSE(IsError(query2.LastError()));
  query2.Bind(0, kDoubleValue);
  // assert
  EXPECT_FALSE(IsError(query2.LastError()));
  EXPECT_TRUE(query2.Exec());
  EXPECT_TRUE(IsDone(query2.LastError()));

  // act
  SQLQuery query3(&db);
  EXPECT_TRUE(query3.Prepare(kInsert3));
  EXPECT_FALSE(IsError(query3.LastError()));
  query3.Bind(0, kStringValue);
  // assert
  EXPECT_FALSE(IsError(query3.LastError()));
  EXPECT_TRUE(query3.Exec());
  EXPECT_TRUE(IsDone(query3.LastError()));

  // act
  SQLQuery query4(&db);
  // assert
  EXPECT_TRUE(query4.Prepare(kInsert4));
  EXPECT_FALSE(IsError(query4.LastError()));
  query4.Bind(0, kIntegerValue);
  query4.Bind(1, kDoubleValue);
  query4.Bind(2, kStringValue);
  // assert
  EXPECT_FALSE(IsError(query4.LastError()));
  EXPECT_TRUE(query4.Exec());
  EXPECT_TRUE(IsDone(query4.LastError()));
}

TEST_F(SQLQueryTest, SetValue_InsertValues_ExpectDBHasInsertedValues) {
  // arrange
  const char* insert =
      "INSERT INTO testTable "
      "(integerValue, doubleValue, stringValue) "
      "VALUES (1, 2.3, 'four');";

  // assert
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, insert, NULL, NULL, NULL));

  // act
  const std::string kSelect(
      "SELECT integerValue, doubleValue, stringValue"
      " FROM testTable");
  const int kIntegerValue = 1;
  const double kDoubleValue = 2.3;
  const std::string kStringValue = "four";

  SQLDatabase db(kDatabaseName);

  // assert
  ASSERT_TRUE(db.Open());

  // act
  SQLQuery query(&db);

  // assert
  EXPECT_TRUE(query.Prepare(kSelect));
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  EXPECT_TRUE(IsRow(query.LastError()));
  EXPECT_EQ(kIntegerValue, query.GetInteger(0));
  EXPECT_EQ(kDoubleValue, query.GetDouble(1));
  EXPECT_EQ(kStringValue, query.GetString(2));
  EXPECT_FALSE(query.Next());
  EXPECT_TRUE(IsDone(query.LastError()));
}

TEST_F(SQLQueryTest, EmptySelect_SelectValuesEqual0_ExecWithoutErrors) {
  // arrange
  const std::string kSelect(
      "SELECT integerValue, doubleValue, stringValue"
      " FROM testTable WHERE 0");
  SQLDatabase db(kDatabaseName);

  // assert
  ASSERT_TRUE(db.Open());

  // act
  SQLQuery query(&db);

  // assert
  EXPECT_TRUE(query.Prepare(kSelect));
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  EXPECT_TRUE(IsDone(query.LastError()));
}

TEST_F(
    SQLQueryTest,
    NextAndBind_InsertValuesAndBindQuery_ExecWithoutErrorsAndBindingQueryIsLast) {
  // arrange
  const char* insert =
      "INSERT INTO testTable "
      "(integerValue, doubleValue, stringValue) "
      "VALUES (1, 2.3, 'four');";

  // assert
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, insert, NULL, NULL, NULL));

  const std::string kSelect(
      "SELECT integerValue, doubleValue, stringValue"
      " FROM testTable WHERE stringValue = ?");

  // act
  const int kIntegerValue = 1;
  const double kDoubleValue = 2.3;
  const std::string kStringValue = "four";

  SQLDatabase db(kDatabaseName);
  ASSERT_TRUE(db.Open());

  SQLQuery query(&db);

  // assert
  ASSERT_TRUE(query.Prepare(kSelect));
  EXPECT_FALSE(IsError(query.LastError()));
  // act
  query.Bind(0, kStringValue);
  // assert
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  ASSERT_TRUE(IsRow(query.LastError()));
  EXPECT_EQ(kIntegerValue, query.GetInteger(0));
  EXPECT_EQ(kDoubleValue, query.GetDouble(1));
  EXPECT_EQ(kStringValue, query.GetString(2));
  EXPECT_FALSE(query.Next());
  EXPECT_TRUE(IsDone(query.LastError()));
}

TEST_F(SQLQueryTest, LastInsertId_InsertValuesAndBindQuery_GetExpectedId) {
  // arrange
  const char* create =
      "CREATE TABLE idTable ( "
      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "value TEXT)";

  // assert
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, create, NULL, NULL, NULL));

  const int64_t kExpectId = 1;
  const std::string kValue("Test last id of insert row");
  const std::string kInsert("INSERT INTO idTable (value) VALUES(?)");

  // act
  SQLDatabase db(kDatabaseName);

  // assert
  ASSERT_TRUE(db.Open());

  // act
  SQLQuery query(&db);

  // assert
  ASSERT_TRUE(query.Prepare(kInsert));
  EXPECT_FALSE(IsError(query.LastError()));

  // act
  query.Bind(0, kValue);
  // assert
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  ASSERT_TRUE(IsDone(query.LastError()));
  EXPECT_EQ(kExpectId, query.LastInsertId());

  ASSERT_EQ(SQLITE_OK,
            sqlite3_exec(conn, "DROP TABLE idTable", NULL, NULL, NULL));
}

TEST_F(SQLQueryTest, BindNull_BindWithoutValue_ActWithoutErrors) {
  // arrange
  const std::string kInsert(
      "INSERT INTO testTable (`integerValue`)"
      " VALUES (?)");
  SQLDatabase db(kDatabaseName);
  // assert
  ASSERT_TRUE(db.Open());

  // act
  SQLQuery query(&db);

  // assert
  ASSERT_TRUE(query.Prepare(kInsert));
  EXPECT_FALSE(IsError(query.LastError()));
  query.Bind(0);
  // assert
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  ASSERT_TRUE(IsDone(query.LastError()));
}

TEST_F(SQLQueryTest, DoublePrepare_TwicePrepareQuery_ActWithoutErrors) {
  // arrange
  SQLDatabase db(kDatabaseName);
  // assert
  ASSERT_TRUE(db.Open());
  // act
  SQLQuery query(&db);
  // assert
  EXPECT_TRUE(query.Prepare("SELECT * FROM testTable"));
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Prepare("SELECT * FROM testTable"));
  EXPECT_FALSE(IsError(query.LastError()));
}

}  // namespace dbms_test
}  // namespace utils_test
}  // namespace components
}  // namespace test
