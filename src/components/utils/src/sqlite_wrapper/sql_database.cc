/*
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

#include "sqlite_wrapper/sql_database.h"
#include <sqlite3.h>

namespace utils {
namespace dbms {

const std::string SQLDatabase::kInMemory = ":memory:";
const std::string SQLDatabase::kExtension = ".sqlite";

SQLDatabase::SQLDatabase()
    : conn_(NULL),
      databasename_(kInMemory),
      error_(SQLITE_OK) {}

SQLDatabase::SQLDatabase(const std::string& db_name)
    : conn_(NULL),
      databasename_(db_name + kExtension),
      error_(SQLITE_OK) {}

SQLDatabase::~SQLDatabase() {
  Close();
}

bool SQLDatabase::Open() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  if (conn_) return true;
  error_ = sqlite3_open(databasename_.c_str(), &conn_);
  return error_ == SQLITE_OK;
}

bool SQLDatabase::IsReadWrite() {
  const char* schema = "main";
  return sqlite3_db_readonly(conn_, schema) == 0;
}

void SQLDatabase::Close() {
  if (!conn_) {
    return;
  }

  sync_primitives::AutoLock auto_lock(conn_lock_);
  error_ = sqlite3_close(conn_);
  if (error_ == SQLITE_OK) {
    conn_ = NULL;
  }
}

bool SQLDatabase::BeginTransaction() {
  return Exec("BEGIN TRANSACTION");
}

bool SQLDatabase::CommitTransaction() {
  return Exec("COMMIT TRANSACTION");
}

bool SQLDatabase::RollbackTransaction() {
  return Exec("ROLLBACK TRANSACTION");
}

bool SQLDatabase::Exec(const std::string& query) {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  error_ = sqlite3_exec(conn_, query.c_str(), NULL, NULL, NULL);
  return error_ == SQLITE_OK;
}

SQLError SQLDatabase::LastError() const {
  return SQLError(Error(error_));
}

sqlite3* SQLDatabase::conn() const {
  return conn_;
}

void SQLDatabase::set_path(const std::string& path) {
  databasename_ = path +  databasename_;
}

std::string SQLDatabase::get_path() const {
  return databasename_;
}

bool SQLDatabase::Backup() {
   return true;
}
}  // namespace dbms
}  // namespace utils
