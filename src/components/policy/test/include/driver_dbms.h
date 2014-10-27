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
#ifndef SRC_COMPONENTS_POLICY_TEST_INCLUDE_DRIVER_DBMS_H_
#define SRC_COMPONENTS_POLICY_TEST_INCLUDE_DRIVER_DBMS_H_

#ifdef __QNX__
#  include <qdb/qdb.h>
#else  // __QNX__
#  include <sqlite3.h>
#endif  // __QNX__

namespace test {
namespace components {
namespace policy {

#ifdef __QNX__
class DBMS {
 public:
  explicit DBMS(std::string db_name) : db_name_(db_name), conn_(0) {
  }
  ~DBMS() {
    Close();
  }
  bool Open() {
    conn_ = qdb_connect(db_name_.c_str(), 0);
    return conn_ != NULL;
  }
  void Close() {
    qdb_disconnect(conn_);
  }
  bool Exec(const char* query) {
    return -1 != qdb_statement(conn_, query);
  }
  int FetchOneInt(const char* query) {
    int stmt = qdb_stmt_init(conn_, query, strlen(query)+1);
    qdb_stmt_exec(conn_, stmt, NULL, 0);
    qdb_result_t* res = qdb_getresult(conn_);
    void* ret = qdb_cell(res, 0, 0);
    int value = 0;
    if (ret) {
      value = *static_cast<int*>(ret);
    }
    qdb_stmt_free(conn_, stmt);
    return value;
  }
  double FetchOneDouble(const char* query) {
    int stmt = qdb_stmt_init(conn_, query, strlen(query)+1);
    qdb_stmt_exec(conn_, stmt, NULL, 0);
    qdb_result_t* res = qdb_getresult(conn_);
    void* ret = qdb_cell(res, 0, 0);
    double value = 0.0;
    if (ret) {
      value = *static_cast<double*>(ret);
    }
    qdb_stmt_free(conn_, stmt);

    return value;
  }
  std::string FetchOneString(const char* query) {
    int stmt = qdb_stmt_init(conn_, query, strlen(query)+1);
    qdb_stmt_exec(conn_, stmt, NULL, 0);
    qdb_result_t* res = qdb_getresult(conn_);
    void* ret = qdb_cell(res, 0, 0);
    std::string value = "";
    if (ret) {
      value = std::string(static_cast<const char*>(ret));
    }
    qdb_stmt_free(conn_, stmt);

    return value;
  }

 private:
  std::string db_name_;
  qdb_hdl_t* conn_;
};

#else  // __QNX__
class DBMS {
 public:
  explicit DBMS(std::string file_name) : file_name_(file_name), conn_(0) {
  }
  ~DBMS() {
    Close();
  }
  bool Open() {
    return SQLITE_OK == sqlite3_open(file_name_.c_str(), &conn_);
  }
  void Close() {
    sqlite3_close(conn_);
    remove(file_name_.c_str());
  }
  bool Exec(const char* query) {
    return SQLITE_OK == sqlite3_exec(conn_, query, NULL, NULL, NULL);
  }
  int FetchOneInt(const char* query) {
    sqlite3_stmt* statement;
    sqlite3_prepare(conn_, query, -1, &statement, NULL);
    sqlite3_step(statement);
    int value = sqlite3_column_int(statement, 0);
    sqlite3_finalize(statement);
    return value;
  }
  double FethcOneDouble(const char* query) {
    sqlite3_stmt* statement;
    sqlite3_prepare(conn_, query, -1, &statement, NULL);
    sqlite3_step(statement);
    double value = sqlite3_column_double(statement, 0);
    sqlite3_finalize(statement);
    return value;
  }
  std::string FetchOneString(const char* query) {
    sqlite3_stmt* statement;
    sqlite3_prepare(conn_, query, -1, &statement, NULL);
    sqlite3_step(statement);
    const unsigned char* txt = sqlite3_column_text(statement, 0);
    std::string value = std::string(reinterpret_cast<const char*>(txt));
    sqlite3_finalize(statement);
    return value;
  }

 private:
  std::string file_name_;
  sqlite3* conn_;
};
#endif  // __QNX__

}  // namespace policy
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_POLICY_TEST_INCLUDE_DRIVER_DBMS_H_
