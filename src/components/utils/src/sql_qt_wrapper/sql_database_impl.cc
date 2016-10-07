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

#include "utils/sql_qt_wrapper/sql_database_impl.h"

#include <QSqlError>

namespace {
const QString kDatabaseExtension = ".sqlite";
}  // namespace

namespace utils {
namespace dbms {

SQLDatabaseImpl::SQLDatabaseImpl() : database_path_() {}

SQLDatabaseImpl::SQLDatabaseImpl(const std::string& database_path,
                                 const std::string& connection_name)
    : database_path_(QString::fromStdString(database_path +
                                            kDatabaseExtension.toStdString()))
    , connection_name_(connection_name.c_str()) {
  db_ = QSqlDatabase::addDatabase("QSQLITE", connection_name_);
}

SQLDatabaseImpl::~SQLDatabaseImpl() {
  Close();
  sync_primitives::AutoLock auto_lock(conn_lock_);
  /*
   * All database queries and connections should be destroyed
   * before database removing. See
   * http://doc.qt.io/qt-5/qsqldatabase.html#removeDatabase
   */
  db_ = QSqlDatabase();
  QSqlDatabase::removeDatabase(connection_name_);
}

bool SQLDatabaseImpl::Open() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  db_.setDatabaseName(database_path_);
  return db_.open();
}

void SQLDatabaseImpl::Close() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  if (db_.isOpen()) {
    db_.close();
  }
}

bool SQLDatabaseImpl::BeginTransaction() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  return db_.transaction();
}

bool SQLDatabaseImpl::CommitTransaction() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  return db_.commit();
}

bool SQLDatabaseImpl::RollbackTransaction() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  return db_.rollback();
}

SQLError SQLDatabaseImpl::LastError() const {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  return SQLError(db_.lastError());
}

bool SQLDatabaseImpl::HasErrors() const {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  return db_.lastError().type() != QSqlError::NoError;
}

void SQLDatabaseImpl::set_path(const std::string& path) {
  database_path_ = path.c_str();
}

std::string SQLDatabaseImpl::get_path() const {
  return database_path_.toStdString();
}

bool SQLDatabaseImpl::IsReadWrite() {
  return true;
}

SQLDatabaseImpl::operator QSqlDatabase() const {
  return db_;
}

bool SQLDatabaseImpl::Backup() {
  return true;
}

}  // namespace dbms
}  // namespace utils
