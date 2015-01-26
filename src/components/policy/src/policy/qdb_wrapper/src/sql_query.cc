/**
 * Copyright (c) 2013, Ford Motor Company
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

#include "qdb_wrapper/sql_query.h"
#include <string.h>
#include <cassert>
#include <algorithm>
#include "qdb_wrapper/sql_database.h"

namespace policy {
namespace dbms {

class SetBindInteger {
 public:
  explicit SetBindInteger(qdb_binding_t* array)
      : array_(array) {
  }
  void operator()(const std::pair<int, int64_t>& x) {
    // In QDB the number of position for binding starts since 1.
    QDB_SETARRAYBIND_INT(array_, x.first + 1, x.second);
  }
 private:
  qdb_binding_t* array_;
};

class SetBindReal {
 public:
  explicit SetBindReal(qdb_binding_t* array)
      : array_(array) {
  }
  void operator()(const std::pair<int, double>& x) {
    // In QDB the number of position for binding starts since 1.
    QDB_SETARRAYBIND_REAL(array_, x.first + 1, x.second);
  }
 private:
  qdb_binding_t* array_;
};

class SetBindText {
 public:
  explicit SetBindText(qdb_binding_t* array)
      : array_(array) {
  }
  void operator()(const std::pair<int, std::string>& x) {
    // In QDB the number of position for binding starts since 1.
    QDB_SETARRAYBIND_TEXT(array_, x.first + 1, x.second.c_str());
  }
 private:
  qdb_binding_t* array_;
};

class SetBindNull {
 public:
  explicit SetBindNull(qdb_binding_t* array)
      : array_(array) {
  }
  void operator()(int x) {
    // In QDB the number of position for binding starts since 1.
    QDB_SETARRAYBIND_NULL(array_, x + 1);
  }
 private:
  qdb_binding_t* array_;
};

SQLQuery::SQLQuery(SQLDatabase* db)
    : db_(db),
      query_(""),
      statement_(-1),
      bindings_(NULL),
      result_(NULL),
      current_row_(0),
      rows_(0),
      error_(Error::OK) {
}

SQLQuery::~SQLQuery() {
  Finalize();
  db_->Close();
  delete db_;
}

bool SQLQuery::Prepare(const std::string& query) {
  query_ = query;
  statement_ = qdb_stmt_init(db_->conn(), query.c_str(), query.length() + 1);
  if (statement_ == -1) {
    error_ = Error::ERROR;
    return false;
  }
  return true;
}

uint8_t SQLQuery::SetBinds() {
  uint8_t binding_count = int_binds_.size() + double_binds_.size()
      + string_binds_.size() + null_binds_.size();

  bindings_ = new qdb_binding_t[binding_count];

  std::for_each(int_binds_.begin(), int_binds_.end(),
                SetBindInteger(bindings_));
  std::for_each(double_binds_.begin(), double_binds_.end(),
                SetBindReal(bindings_));
  std::for_each(string_binds_.begin(), string_binds_.end(),
                SetBindText(bindings_));
  std::for_each(null_binds_.begin(), null_binds_.end(), SetBindNull(bindings_));

  return binding_count;
}

bool SQLQuery::Result() {
  result_ = qdb_getresult(db_->conn());
  if (!result_) {
    error_ = Error::ERROR;
    return false;
  }
  rows_ = qdb_rows(result_);
  if (rows_ == -1) {
    rows_ = 0;
    error_ = Error::ERROR;
    return false;
  }
  return true;
}

bool SQLQuery::Exec() {
  sync_primitives::AutoLock auto_lock(bindings_lock_);
  if (result_)
    return true;

  current_row_ = 0;
  uint8_t binding_count = SetBinds();
  if (qdb_stmt_exec(db_->conn(), statement_, bindings_, binding_count) == -1) {
    error_ = Error::ERROR;
    return false;
  }
  return Result();
}

bool SQLQuery::Next() {
  ++current_row_;
  return Exec() && current_row_ < rows_;
}

bool SQLQuery::Reset() {
  sync_primitives::AutoLock auto_lock(bindings_lock_);
  int_binds_.clear();
  double_binds_.clear();
  string_binds_.clear();
  null_binds_.clear();
  delete[] bindings_;
  bindings_ = NULL;
  rows_ = 0;
  current_row_ = 0;
  if (result_ && qdb_freeresult(result_) == -1) {
    error_ = Error::ERROR;
    return false;
  }
  result_ = NULL;
  return true;
}

void SQLQuery::Finalize() {
  if (Reset() && qdb_stmt_free(db_->conn(), statement_) != -1) {
    statement_ = 0;
  } else {
    error_ = Error::ERROR;
  }
}

bool SQLQuery::Exec(const std::string& query) {
  query_ = query;
  if (qdb_statement(db_->conn(), query.c_str()) == -1) {
    error_ = Error::ERROR;
    return false;
  }
  return true;
}

void SQLQuery::Bind(int pos, int value) {
  int_binds_.push_back(std::make_pair(pos, value));
}

void SQLQuery::Bind(int pos, int64_t value) {
  int_binds_.push_back(std::make_pair(pos, value));
}

void SQLQuery::Bind(int pos, double value) {
  double_binds_.push_back(std::make_pair(pos, value));
}

void SQLQuery::Bind(int pos, bool value) {
  Bind(pos, static_cast<int>(value));
}

void SQLQuery::Bind(int pos, const std::string& value) {
  string_binds_.push_back(std::make_pair(pos, value));
}

void SQLQuery::Bind(int pos) {
  null_binds_.push_back(pos);
}

bool SQLQuery::GetBoolean(int pos) const {
  return static_cast<bool>(GetInteger(pos));
}

int SQLQuery::GetInteger(int pos) const {
  void* ret = qdb_cell(result_, current_row_, pos);
  if (rows_ !=0 && ret) {
    return *static_cast<int*>(ret);
  }
  return 0;
}

double SQLQuery::GetDouble(int pos) const {
  void* ret = qdb_cell(result_, current_row_, pos);
  if (rows_ !=0 && ret) {
    return *static_cast<double*>(ret);
  }
  return 0;
}

std::string SQLQuery::GetString(int pos) const {
  void* ret = qdb_cell(result_, current_row_, pos);
  if (rows_ != 0 && ret) {
    return static_cast<const char*>(ret);
  }
  return "";
}

bool SQLQuery::IsNull(int pos) const {
  return rows_ == 0 || qdb_cell_type(result_, current_row_, pos) == QDB_NULL;
}

const std::string& SQLQuery::query() const {
  // TODO(KKolodiy): may return string query with value of arguments
  return query_;
}

SQLError SQLQuery::LastError() const {
  return SQLError(error_, qdb_geterrmsg(db_->conn()));
}

int64_t SQLQuery::LastInsertId() const {
  return qdb_last_insert_rowid(db_->conn(), result_);
}

}  // namespace dbms
}  // namespace policy

