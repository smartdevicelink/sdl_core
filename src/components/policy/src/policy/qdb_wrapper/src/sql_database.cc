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

#include <cstring>
#include "qdb_wrapper/sql_database.h"
#include "utils/logger.h"

namespace policy {
namespace dbms {

CREATE_LOGGERPTR_GLOBAL(logger_, "SQLDatabase")

SQLDatabase::SQLDatabase(const std::string& db_name)
    : conn_(NULL),
      db_name_(db_name),
      error_(Error::OK) {
}

SQLDatabase::~SQLDatabase() {
  Close();
}

bool SQLDatabase::Open() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  if (conn_) return true;
  conn_ = qdb_connect(db_name_.c_str(), 0);
  if (conn_ == NULL) {
    error_ = Error::ERROR;
    return false;
  }
  return true;
}

void SQLDatabase::Close() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  if (conn_) {
    if (qdb_disconnect(conn_) != -1) {
      conn_ = NULL;
    } else {
      error_ = Error::ERROR;
    }
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
  if (qdb_statement(conn_, query.c_str()) == -1) {
    error_ = Error::ERROR;
    return false;
  }
  return true;
}

SQLError SQLDatabase::LastError() const {
  return SQLError(error_, qdb_geterrmsg(conn_));
}

qdb_hdl_t* SQLDatabase::conn() const {
  return conn_;
}

bool SQLDatabase::Backup() {
  if (qdb_backup(conn_, QDB_ATTACH_DEFAULT) == -1) {
	error_ = Error::ERROR;
    LOG4CXX_ERROR(logger_, "Backup returned error: " << std::strerror(errno));
	return false;
  }
  LOG4CXX_INFO(logger_, "Backup was successful.");
  return true;
}

}  // namespace dbms
}  // namespace policy
