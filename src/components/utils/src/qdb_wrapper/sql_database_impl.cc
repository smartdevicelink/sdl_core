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

#include <cstring>
#include "utils/qdb_wrapper/sql_database_impl.h"
#include "utils/logger.h"

namespace utils {
namespace dbms {

SDL_CREATE_LOGGER("SQLDatabase")

SQLDatabaseImpl::SQLDatabaseImpl(const std::string& db_name)
    : conn_(NULL), db_name_(db_name), error_(Error::OK) {}

SQLDatabaseImpl::~SQLDatabaseImpl() {
  Close();
}

bool SQLDatabaseImpl::Open() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  if (conn_)
    return true;
  conn_ = qdb_connect(db_name_.c_str(), 0);
  if (conn_ == NULL) {
    error_ = Error::ERROR;
    return false;
  }
  return true;
}

void SQLDatabaseImpl::Close() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  if (conn_) {
    if (qdb_disconnect(conn_) != -1) {
      conn_ = NULL;
    } else {
      error_ = Error::ERROR;
    }
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
  if (qdb_statement(conn_, query.c_str()) == -1) {
    error_ = Error::ERROR;
    return false;
  }
  return true;
}

SQLError SQLDatabaseImpl::LastError() const {
  return SQLError(error_, qdb_geterrmsg(conn_));
}

bool SQLDatabase::HasErrors() const {
  return Error(error_) != OK;
}

qdb_hdl_t* SQLDatabaseImpl::conn() const {
  return conn_;
}

std::string SQLDatabaseImpl::get_path() const {
  return db_name_;
}

bool SQLDatabaseImpl::Backup() {
  if (qdb_backup(conn_, QDB_ATTACH_DEFAULT) == -1) {
    error_ = Error::ERROR;
    SDL_ERROR("Backup returned error: " << std::strerror(errno));
    return false;
  }
  SDL_INFO("Backup was successful.");
  return true;
}

}  // namespace dbms
}  // namespace utils
