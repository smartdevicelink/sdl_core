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

#include <sqlite3.h>
#include "gtest/gtest.h"
#include "generated_code_with_sqlite_test.h"

namespace rpc {

class GeneratedCodeTest : public ::testing::Test {
 public:
  static void SetUpTestCase() {
    sqlite3* conn;
    sqlite3_open((kDatabaseName + ".sqlite").c_str(), &conn);
    sqlite3_exec(conn, kEndpointsCreation.c_str(), NULL, NULL, NULL);
    sqlite3_exec(conn, kEndpointsContent.c_str(), NULL, NULL, NULL);
    sqlite3_exec(conn, kAppPoliciesCreation.c_str(), NULL, NULL, NULL);
    sqlite3_exec(conn, kGroupsCreation.c_str(), NULL, NULL, NULL);
    sqlite3_close(conn);
  }

  static void TearDownTestCase() {
    remove((kDatabaseName + ".sqlite").c_str());
  }

  static const std::string kDatabaseName;
  static const std::string kEndpointsCreation;
  static const std::string kEndpointsContent;
  static const std::string kAppPoliciesCreation;
  static const std::string kGroupsCreation;
};

const std::string GeneratedCodeTest::kDatabaseName = "test_db";

const std::string GeneratedCodeTest::kEndpointsCreation =
    "CREATE TABLE Endpoints ("
        "endpoint_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
        "service_id VARCHAR(45) NOT NULL,"
        "application_id VARCHAR(45),"
        "url VARCHAR(45) NOT NULL,"
        "is_default INTEGER NOT NULL CHECK(is_default>=0))";

const std::string GeneratedCodeTest::kEndpointsContent =
    "INSERT INTO Endpoints "
        "VALUES (1, '0x07', null, 'http://url.example.com', 1)";

const std::string GeneratedCodeTest::kAppPoliciesCreation =
    "CREATE TABLE AppPolicies ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
        "application_id VARCHAR(45),"
        "priority VARCHAR(45),"
        "is_default INTEGER NOT NULL CHECK(is_default>=0))";

const std::string GeneratedCodeTest::kGroupsCreation = "CREATE TABLE Groups ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
    "application_id VARCHAR(45) NOT NULL,"
    "group_name VARCHAR(45) NOT NULL )";

TEST_F(GeneratedCodeTest, FindSectionEndpoints_OpenDBSetDefaultUrl_ExpectDefaultUrl) {

  //arrange
  dbms::SQLDatabase db(GeneratedCodeTest::kDatabaseName);

  //assert
  EXPECT_TRUE(db.Open());
  policy_table::ServiceEndpoints ep;

  //assert
  EXPECT_TRUE(policy_table::FindSection(&db, ep));
  EXPECT_EQ(1u, ep.size());

  //act
  std::string url = ep["0x07"]["default"].front();

  //assert
  EXPECT_EQ("http://url.example.com", url);
}

TEST_F(GeneratedCodeTest, RemoveSectionEndpoints_RemoveSectionEndpoints_Expect0EndPoints) {
  //arrange
  dbms::SQLDatabase db(GeneratedCodeTest::kDatabaseName);

  //assert
  EXPECT_TRUE(db.Open());

  //act
  policy_table::ServiceEndpoints ep;

  //assert
  EXPECT_TRUE(policy_table::RemoveSection(&db, ep));
  dbms::SQLQuery sqlquery(&db);

  //act
  std::string check_query = "select count (*) from endpoints";

  //assert
  EXPECT_TRUE(sqlquery.Prepare(check_query));
  EXPECT_TRUE(sqlquery.Exec());
  // Index for binding starts from 1, index for results starts from 0
  EXPECT_EQ(0, sqlquery.GetInteger(0));
}

TEST_F(GeneratedCodeTest, UpdateSectionEndpoints_SetUrlPoint_ExpectPointEqualsUrl) {

  //arrange
  dbms::SQLDatabase db(GeneratedCodeTest::kDatabaseName);

  //assert
  EXPECT_TRUE(db.Open());

  //act
  std::string test_url = "http://url.example.com";

  policy_table::URL urls;
  urls.push_back(test_url);

  policy_table::URLList urllist;
  urllist["default"] = urls;

  policy_table::ServiceEndpoints ep;
  ep["0x07"] = urllist;

  //assert
  EXPECT_TRUE(policy_table::UpdateSection(&db, ep));

  dbms::SQLQuery sqlquery(&db);
  std::string num_of_records_check = "select count (*) from endpoints";

  //assert
  EXPECT_TRUE(sqlquery.Prepare(num_of_records_check));
  EXPECT_TRUE(sqlquery.Exec());
  // Index for binding starts from 1, index for results starts from 0
  EXPECT_EQ(1, sqlquery.GetInteger(0));
  EXPECT_TRUE(sqlquery.Reset());


  //act
  std::string url_check_query = "select * from endpoints";

  //assert
  EXPECT_TRUE(sqlquery.Prepare(url_check_query));
  EXPECT_TRUE(sqlquery.Exec());
  // Index for binding starts from 1, index for results starts from 0
  EXPECT_EQ(test_url, sqlquery.GetString(3));
}

TEST_F(GeneratedCodeTest, UpdateSectionAppPolicies_SetAppParams_ExpectDBHasThem) {

  //arrange
  dbms::SQLDatabase db(GeneratedCodeTest::kDatabaseName);

  //assert
  EXPECT_TRUE(db.Open());

  //act
  policy_table::ApplicationPolicies ap;
  const std::string application_id = "12345678";
  ap[application_id].groups.push_back("Base-4");
  ap[application_id].priority = policy_table::P_NORMAL;

  //assert
  EXPECT_TRUE(policy_table::UpdateSection(&db, ap));

  //act
  dbms::SQLQuery sqlquery(&db);


  //assert
  EXPECT_TRUE(sqlquery.Prepare("select count (*) from AppPolicies"));
  EXPECT_TRUE(sqlquery.Exec());
  // Index for binding starts from 1, index for results starts from 0
  EXPECT_EQ(1, sqlquery.GetInteger(0));
  EXPECT_TRUE(sqlquery.Reset());

  EXPECT_TRUE(sqlquery.Prepare("select count (*) from Groups"));
  EXPECT_TRUE(sqlquery.Exec());
  // Index for binding starts from 1, index for results starts from 0
  EXPECT_EQ(1, sqlquery.GetInteger(0));
  EXPECT_TRUE(sqlquery.Reset());

  EXPECT_TRUE(
      sqlquery.Prepare(
          "select application_id from Groups where group_name='Base-4'"));
  EXPECT_TRUE(sqlquery.Exec());
  // Index for binding starts from 1, index for results starts from 0
  EXPECT_EQ(application_id, sqlquery.GetString(0));
  EXPECT_TRUE(sqlquery.Reset());
}

}  // namespace rpc
