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

#include "sqlite_wrapper/sql_database_impl.h"
#include <sqlite3.h>

namespace utils {
namespace dbms {

namespace {
const char* kInMemory = ":memory:";
const char* kExtension = ".sqlite";
}  // namespace

SQLDatabaseImpl::SQLDatabaseImpl()
    : conn_(NULL), database_name_(kInMemory), error_(SQLITE_OK) {}

SQLDatabaseImpl::SQLDatabaseImpl(const std::string& db_name)
    : conn_(NULL), database_name_(db_name + kExtension), error_(SQLITE_OK) {}

SQLDatabaseImpl::~SQLDatabaseImpl() {
  Close();
}

bool SQLDatabaseImpl::Open() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  if (conn_) {
    return true;
  }
  error_ = sqlite3_open(database_name_.c_str(), &conn_);
  return error_ == SQLITE_OK;
  return true;
}

bool SQLDatabaseImpl::IsReadWrite() {
  const char* schema = "main";
  return sqlite3_db_readonly(conn_, schema) == 0;
  return true;
}

void SQLDatabaseImpl::Close() {
  if (!conn_) {
    return;
  }

  sync_primitives::AutoLock auto_lock(conn_lock_);
  error_ = sqlite3_close(conn_);
  if (error_ == SQLITE_OK) {
    conn_ = NULL;
  }
}

bool SQLDatabaseImpl::BeginTransaction() {
  return Exec("BEGIN TRANSACTION");
}

bool SQLDatabaseImpl::CommitTransaction() {
  return Exec("COMMIT TRANSACTION");
}

bool SQLDatabaseImpl::RollbackTransaction() {
  return Exec("ROLLBACK TRANSACTION");
}

bool SQLDatabaseImpl::Exec(const std::string& query) {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  error_ = sqlite3_exec(conn_, query.c_str(), NULL, NULL, NULL);
  return error_ == SQLITE_OK;
}

SQLError SQLDatabaseImpl::LastError() const {
  return SQLError(Error(error_));
}

bool SQLDatabaseImpl::HasErrors() const {
  return Error(error_) != OK;
}

sqlite3* SQLDatabaseImpl::conn() const {
  return conn_;
}

void SQLDatabaseImpl::set_path(const std::string& path) {
  database_name_ = path + database_name_;
}

std::string SQLDatabaseImpl::get_path() const {
  return database_name_;
}

bool SQLDatabaseImpl::Backup() {
  return true;
}
}  // namespace dbms
}  // namespace utils
