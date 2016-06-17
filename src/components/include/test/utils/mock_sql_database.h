/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_UTILS_MOCK_SQL_DATABASE_H_
#define SRC_COMPONENTS_INCLUDE_TEST_UTILS_MOCK_SQL_DATABASE_H_

#include <string>

#include "utils/sql_database.h"

struct sqlite3;

namespace test {
namespace components {
namespace utils {
namespace dbms {

class MockSQLDatabase : public ::utils::dbms::SQLDatabase {
 public:
  MOCK_METHOD0(Open, bool());
  MOCK_METHOD0(Close, void());
  MOCK_METHOD0(BeginTransaction, bool());
  MOCK_METHOD0(CommitTransaction, bool());
  MOCK_METHOD0(RollbackTransaction, bool());
  MOCK_CONST_METHOD0(LastError, ::utils::dbms::SQLError());
  MOCK_METHOD1(set_path, void(const std::string& path));
  MOCK_CONST_METHOD0(get_path, std::string());
  MOCK_METHOD0(IsReadWrite, bool());
  MOCK_METHOD0(Backup, bool());
  MOCK_CONST_METHOD0(conn, sqlite3*());
  MOCK_CONST_METHOD0(HasErrors, bool());
};

}  // namespace dbms
}  // namespace utils
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_UTILS_MOCK_SQL_DATABASE_H_
