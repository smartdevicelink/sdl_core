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

#ifndef SRC_COMPONENTS_POLICY_SQLITE_WRAPPER_INCLUDE_SQLITE_WRAPPER_SQL_QUERY_H_
#define SRC_COMPONENTS_POLICY_SQLITE_WRAPPER_INCLUDE_SQLITE_WRAPPER_SQL_QUERY_H_

#include <stdint.h>
#include <string>
#include "sqlite_wrapper/sql_error.h"
#include "utils/lock.h"

struct sqlite3_stmt;

namespace policy {
namespace dbms {

class SQLDatabase;

/**
 * Provides a means of executing and manipulating SQL statements
 */
class SQLQuery {
 public:
  explicit SQLQuery(SQLDatabase* db);
  ~SQLQuery();

  /**
   * Prepares the SQL query for executing
   * @param query the utf-8 string of SQL query
   * @return true if successfully
   */
  bool Prepare(const std::string& query);

  /**
   * Resets the binds of query for re-executing
   * @return true if successfully
   */
  bool Reset();

  /**
   * Deletes prepared SQL query
   */
  void Finalize();

  /**
   * Executes SQL query without make binds
   * @param query the utf-8 string of SQL query
   * @return true if successfull
   */
  bool Exec(const std::string& query);

  /**
   * Executes prepared SQL query and positions the query on the first record
   * @return true if successfull
   */
  bool Exec();

  /**
   * Retrieves the next record in the result, if available,
   * and positions the query on the retrieved record
   * @return true if record was retrieved successfully, false if a error was
   * or the result is empty or was retrieves last record
   */
  bool Next();

  /**
   * Binds null in the prepared query
   * @param pos position of param in the query
   */
  void Bind(int pos);

  /**
   * Binds int value in the prepared query.
   * @param pos position of param in the query
   * @param value value of param
   */
  void Bind(int pos, int value);

  /**
   * Binds int64_t value in the prepared query.
   * @param pos position of param in the query
   * @param value value of param
   */
  void Bind(int pos, int64_t value);

  /**
   * Binds double value in the prepared query.
   * @param pos position of param in the query
   * @param value value of param
   */
  void Bind(int pos, double value);

  /**
   * Binds bool value in the prepared query.
   * @param pos position of param in the query
   * @param value value of param
   */
  void Bind(int pos, bool value);

  /**
   * Binds string in the prepared query.
   * @param pos position of param in the query
   * @param value utf-8 string
   */
  void Bind(int pos, const std::string& value);

  /**
   * Gets value in the result record
   * @param pos position of value
   * @return boolean value
   */
  bool GetBoolean(int pos) const;

  /**
   * Gets value in the result record
   * @param pos position of value
   * @return integer value
   */
  int GetInteger(int pos) const;

  /**
   * Gets value in the result record
   * @param pos position of value
   * @return double value
   */
  double GetDouble(int pos) const;

  /**
   * Gets value in the result record
   * @param pos position of value
   * @return string value
   */
  std::string GetString(int pos) const;

  /**
   * Checks if value is null
   * @param pos position of value
   * @return true if value is null
   */
  bool IsNull(int pos) const;

  /**
   * Gets last id of insert row
   * @return id of insert row
   */
  int64_t LastInsertId() const;

  /**
   * Gets string of the query
   * @return string of the query
   */
  const std::string& query() const;

  /**
   * Gets information about the last error that occurred on the database
   * @return last error
   */
  SQLError LastError() const;

 private:
  /**
   * The instantiation of database
   */
  SQLDatabase& db_;

  /**
   * The string of query
   */
  std::string query_;

  /**
   * The SQL statement in SQLite
   */
  sqlite3_stmt* statement_;

  /**
   * Lock for guarding statement
   */
  sync_primitives::Lock statement_lock_;

  /**
   * The last error that occurred with this query
   */
  int error_;
};

}  // namespace dbms
}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_SQLITE_WRAPPER_INCLUDE_SQLITE_WRAPPER_SQL_QUERY_H_
