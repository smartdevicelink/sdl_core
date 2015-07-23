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

#include <qdb/qdb.h>
#include <string>

#include "gtest/gtest.h"

#include "qdb_wrapper/sql_error.h"
#include "qdb_wrapper/sql_database.h"
#include "qdb_wrapper/sql_query.h"

using ::utils::dbms::SQLError;
using ::utils::dbms::SQLDatabase;
using ::utils::dbms::SQLQuery;

namespace test {
namespace components {
namespace utils {
namespace dbms {

class SQLQueryTest : public ::testing::Test {
 protected:
  static qdb_hdl_t* conn;
  static const std::string kDatabaseName;

  static void SetUpTestCase() {
    conn = qdb_connect(kDatabaseName.c_str(), 0);
    ASSERT_TRUE(conn);
    int ret = qdb_statement(conn, "CREATE TABLE testTable (integerValue INTEGER,"
                 " doubleValue REAL, stringValue TEXT)");
    ASSERT_NE(-1, ret);
  }

  static void TearDownTestCase() {
    qdb_statement(conn, "DROP TABLE IF EXISTS testTable");
    qdb_disconnect(conn);
  }

  void SetUp() {
    qdb_statement(conn, "DELETE FROM testTable");
  }

  ::testing::AssertionResult IsError(SQLError error) {
    if (error.number() != ::policy::dbms::OK) {
      return ::testing::AssertionSuccess() << error.text();
    } else {
      return ::testing::AssertionFailure() << error.text();
    }
  }
};

qdb_hdl_t* SQLQueryTest::conn = 0;
const std::string SQLQueryTest::kDatabaseName = "test-query";

TEST_F(SQLQueryTest, Query) {
  const std::string kSelect("SELECT * FROM testTable WHERE integerValue = ?");

  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());

  SQLQuery query(db);
  query.Prepare(kSelect);
  EXPECT_STREQ(kSelect.c_str(), query.query().c_str());
}

TEST_F(SQLQueryTest, ExecString) {
  const std::string kInsert("INSERT INTO testTable"
                            " (integerValue, doubleValue, stringValue)"
                            " VALUES(1, 1.1, 'one-один')");

  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());

  SQLQuery query(db);
  EXPECT_TRUE(query.Exec(kInsert));
  EXPECT_FALSE(IsError(query.LastError()));
}

TEST_F(SQLQueryTest, BindInteger) {
  const std::string kInsert("INSERT INTO testTable (integerValue) VALUES (?)");
  const int kIntegerValue = 2;

  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());

  SQLQuery query(db);
  EXPECT_TRUE(query.Prepare(kInsert));
  EXPECT_FALSE(IsError(query.LastError()));
  query.Bind(0, kIntegerValue);
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  EXPECT_FALSE(IsError(query.LastError()));
}

TEST_F(SQLQueryTest, BindDouble) {
  const std::string kInsert("INSERT INTO testTable (doubleValue) VALUES (?)");
  const double kDoubleValue = 3.3;

  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());
  SQLQuery query(db);
  EXPECT_TRUE(query.Prepare(kInsert));
  EXPECT_FALSE(IsError(query.LastError()));
  query.Bind(0, kDoubleValue);
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  EXPECT_FALSE(IsError(query.LastError()));
}

TEST_F(SQLQueryTest, BindString) {
  const std::string kInsert("INSERT INTO testTable (stringValue) VALUES (?)");
  const std::string kStringValue = "four-четыре";

  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());

  SQLQuery query(db);
  EXPECT_TRUE(query.Prepare(kInsert));
  EXPECT_FALSE(IsError(query.LastError()));
  query.Bind(0, kStringValue);
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  EXPECT_FALSE(IsError(query.LastError()));
}

TEST_F(SQLQueryTest, BindAllTypes) {
  const std::string kInsert("INSERT INTO testTable (integerValue, doubleValue,"
                             " stringValue) VALUES (?, ?, ?)");
  const int kIntegerValue = 5;
  const double kDoubleValue = 5.5;
  const std::string kStringValue = "five-пять";

  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());

  SQLQuery query(db);
  EXPECT_TRUE(query.Prepare(kInsert));
  EXPECT_FALSE(IsError(query.LastError()));
  query.Bind(0, kIntegerValue);
  EXPECT_FALSE(IsError(query.LastError()));
  query.Bind(1, kDoubleValue);
  EXPECT_FALSE(IsError(query.LastError()));
  query.Bind(2, kStringValue);
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  EXPECT_FALSE(IsError(query.LastError()));
}

TEST_F(SQLQueryTest, Value) {
  const char* insert = "INSERT INTO testTable "
      "(integerValue, doubleValue, stringValue) "
      "VALUES (6, 6.6, 'six-шесть');";
  ASSERT_NE(-1, qdb_statement(conn, insert));

  const std::string kSelect("SELECT integerValue, doubleValue, stringValue"
                            " FROM testTable WHERE integerValue = 6");
  const int kIntegerValue = 6;
  const double kDoubleValue = 6.6;
  const std::string kStringValue = "six-шесть";

  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());

  SQLQuery query(db);
  EXPECT_TRUE(query.Prepare(kSelect));
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_EQ(kIntegerValue, query.GetInteger(0));
  EXPECT_EQ(kDoubleValue, query.GetDouble(1));
  EXPECT_EQ(kStringValue, query.GetString(2));
  EXPECT_FALSE(query.Next());
  EXPECT_FALSE(IsError(query.LastError()));
}

TEST_F(SQLQueryTest, EmptySelect) {
  const std::string kSelect("SELECT integerValue, doubleValue, stringValue"
                            " FROM testTable WHERE 0");
  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());

  SQLQuery query(db);
  EXPECT_TRUE(query.Prepare(kSelect));
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  EXPECT_FALSE(IsError(query.LastError()));
}

TEST_F(SQLQueryTest, NextAndBind) {
  const char* insert = "INSERT INTO testTable "
      "(integerValue, doubleValue, stringValue) "
      "VALUES (7, 7.7, 'seven-семь');";
  ASSERT_NE(-1, qdb_statement(conn, insert));

  const std::string kSelect("SELECT integerValue, doubleValue, stringValue"
                            " FROM testTable WHERE integerValue = ?");
  const int kIntegerValue = 7;
  const double kDoubleValue = 7.7;
  const std::string kStringValue = "seven-семь";

  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());

  SQLQuery query(db);
  ASSERT_TRUE(query.Prepare(kSelect));
  EXPECT_FALSE(IsError(query.LastError()));
  query.Bind(0, kIntegerValue);
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  ASSERT_FALSE(IsError(query.LastError()));
  EXPECT_EQ(kIntegerValue, query.GetInteger(0));
  EXPECT_EQ(kDoubleValue, query.GetDouble(1));
  EXPECT_EQ(kStringValue, query.GetString(2));
  EXPECT_FALSE(query.Next());
  EXPECT_FALSE(IsError(query.LastError()));
}

TEST_F(SQLQueryTest, LastInsertId) {
  const char* create = "CREATE TABLE idTable ( "
      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "value TEXT)";
  ASSERT_NE(-1, qdb_statement(conn, create));

  const int64_t kExpectId = 1;
  const std::string kValue("Test last id of insert row");
  const std::string kInsert("INSERT INTO idTable (value) VALUES(?)");

  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());

  SQLQuery query(db);
  ASSERT_TRUE(query.Prepare(kInsert));
  EXPECT_FALSE(IsError(query.LastError()));
  query.Bind(0, kValue);
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  ASSERT_FALSE(IsError(query.LastError()));
  EXPECT_EQ(kExpectId, query.LastInsertId());

  ASSERT_NE(-1, qdb_statement(conn, "DROP TABLE idTable"));
}

TEST_F(SQLQueryTest, BindNull) {
  const std::string kInsert("INSERT INTO testTable (`integerValue`)"
                            " VALUES (?)");
  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());

  SQLQuery query(db);
  ASSERT_TRUE(query.Prepare(kInsert));
  EXPECT_FALSE(IsError(query.LastError()));
  query.Bind(0);
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Exec());
  ASSERT_FALSE(IsError(query.LastError()));
}

TEST_F(SQLQueryTest, DoublePrepare) {
  SQLDatabase* db = new SQLDatabase(kDatabaseName);
  ASSERT_TRUE(db->Open());

  SQLQuery query(db);
  EXPECT_TRUE(query.Prepare("SELECT * FROM testTable"));
  EXPECT_FALSE(IsError(query.LastError()));
  EXPECT_TRUE(query.Prepare("SELECT * FROM testTable"));
  EXPECT_FALSE(IsError(query.LastError()));
}

}  // namespace dbms
}  // namespace utils
}  // namespace components
}  // namespace test
