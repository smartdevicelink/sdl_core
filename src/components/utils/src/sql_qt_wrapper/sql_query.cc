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

#include "sql_qt_wrapper/sql_query.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QStringList>
#include <QVariant>

#include <cassert>
#include <limits>

#include "utils/sql_database.h"
#include "utils/macro.h"

namespace utils {
namespace dbms {

// Accroding to the QSqlQuery documentation
// the query should be moved to the next position after execution
// in order to receive data using `QSqlQuery::value` method
// In order to distinguish that value could be obtained
// it is required to check if current query is active
// and the pointer position is invalid which is means `before first record`
namespace {

void PreparePullValue(QSqlQuery& query) {
  if (query.isActive() && !query.isValid()) {
    query.next();
  }
}

}  // namespace

SQLQuery::SQLQuery(SQLDatabase* db)
    : query_(static_cast<QSqlDatabase>(*db)), queries_cache_() {}

SQLQuery::~SQLQuery() {
  Finalize();
}

bool SQLQuery::Prepare(const std::string& query) {
  Finalize();
  const QStringList& list = SplitQuery(query);
  if (1 == list.size()) {
    return query_.prepare(list[0]);
  }
  queries_cache_ = list;
  return true;
}

bool SQLQuery::Exec() {
  if (queries_cache_.empty()) {
    return query_.exec();
  }
  foreach (QString q, queries_cache_) {
    if (!query_.exec(q)) {
      queries_cache_.clear();
      return false;
    }
  }
  queries_cache_.clear();
  return true;
}

bool SQLQuery::Next() {
  // According to the Qt documentation the `next()` without
  // `exec()` will do nothing and return false.
  // In order to avoid this need to check the `exec()` has been already
  // called by checking `isActive()` state.
  if (!query_.isActive()) {
    return query_.exec() && query_.next();
  }
  return query_.next();
}

bool SQLQuery::Reset() {
  // Need too clear query until use it again with new data
  Finalize();
  return true;
}

void SQLQuery::Finalize() {
  queries_cache_.clear();
  query_.finish();
}

bool SQLQuery::Exec(const std::string& query) {
  foreach (QString q, SplitQuery(query)) {
    if (!query_.exec(q)) {
      return false;
    }
  }
  return true;
}

bool SQLQuery::GetBoolean(int pos) {
  PreparePullValue(query_);
  const QVariant val = query_.value(pos);
  return val.toBool();
}

int SQLQuery::GetInteger(int pos) {
  PreparePullValue(query_);
  const QVariant val = query_.value(pos);
  return val.toInt();
}

uint32_t SQLQuery::GetUInteger(int pos) {
  PreparePullValue(query_);
  const QVariant val = query_.value(pos);
  return val.toUInt();
}

int64_t SQLQuery::GetLongInt(int pos) {
  PreparePullValue(query_);
  const QVariant val = query_.value(pos);
  const qulonglong value = val.toULongLong();
  const qulonglong max_value =
      static_cast<qulonglong>(std::numeric_limits<int64_t>::max());

  DCHECK_OR_RETURN(value <= max_value, max_value);

  return static_cast<int64_t>(value);
}

double SQLQuery::GetDouble(int pos) {
  PreparePullValue(query_);
  const QVariant val = query_.value(pos);
  return val.toDouble();
}

std::string SQLQuery::GetString(int pos) {
  PreparePullValue(query_);
  const QVariant val = query_.value(pos);
  return val.toString().toStdString();
}

std::string SQLQuery::query() const {
  return query_.lastQuery().toStdString();
}

bool SQLQuery::IsNull(int pos) {
  PreparePullValue(query_);
  return query_.value(pos).isNull();
}

void SQLQuery::Bind(int pos, int value) {
  query_.bindValue(pos, QVariant::fromValue(value));
}

void SQLQuery::Bind(int pos, int64_t value) {
  query_.bindValue(pos, QVariant::fromValue(value));
}

void SQLQuery::Bind(int pos, double value) {
  query_.bindValue(pos, QVariant::fromValue(value));
}

void SQLQuery::Bind(int pos, bool value) {
  query_.bindValue(pos, QVariant::fromValue(value));
}

void SQLQuery::Bind(int pos, const std::string& value) {
  query_.bindValue(pos, QVariant(value.c_str()));
}

void SQLQuery::Bind(int pos) {
  query_.bindValue(pos, QVariant(QVariant::String));
}

SQLError SQLQuery::LastError() const {
  return SQLError(query_.lastError());
}

int64_t SQLQuery::LastInsertId() const {
  const QVariant val = query_.lastInsertId();
  return val.toLongLong();
}

QStringList SQLQuery::SplitQuery(const std::string& query) const {
  // QSqlQuery is unable to process several statements
  // in one string, so need to split queries and execute them one by one
  QString qstr = QString::fromStdString(query).trimmed();
  return qstr.split(";", QString::SkipEmptyParts);
}

}  // namespace dbms
}  // namespace utils
