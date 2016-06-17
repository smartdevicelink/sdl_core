/*
 * Copyright (c) 2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SQLITE_WRAPPER_SQL_DATABASE_IMPL_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SQLITE_WRAPPER_SQL_DATABASE_IMPL_H_

#include <string>

#include "utils/sql_database.h"
#include "utils/sqlite_wrapper/sql_error.h"
#include "utils/lock.h"

namespace utils {
namespace dbms {
/**
 * Represents a connection to a database.
 */
class SQLDatabaseImpl : public SQLDatabase {
 public:
  SQLDatabaseImpl();
  explicit SQLDatabaseImpl(const std::string& filename);
  ~SQLDatabaseImpl();

  /**
   * Opens connection to the temporary in-memory database
   * @return true if successfully
   */
  bool Open() OVERRIDE;

  /**
   * Closes connection to the database
   */
  void Close() OVERRIDE;

  /**
   * Begins a transaction on the database
   * @return true if successfully
   */
  bool BeginTransaction() OVERRIDE;

  /**
   * Commits a transaction to the database
   * @return true if successfully
   */
  bool CommitTransaction() OVERRIDE;

  /**
   * Rolls back a transaction on the database
   * @return true if successfully
   */
  bool RollbackTransaction() OVERRIDE;

  /**
   * Gets information about the last error that occurred on the database
   * @return last error
   */
  SQLError LastError() const OVERRIDE;

  /**
   * @brief HasErrors Indicate the status of the last executed operation.
   *
   * @return true in case last operation has any errors, false otherwise.
   */
  bool HasErrors() const OVERRIDE;

  void set_path(const std::string& path) OVERRIDE;

  /**
   * @brief get_path databse location path.
   *
   * @return the path to the database location
   */
  std::string get_path() const OVERRIDE;

  /**
   * Checks if database is read/write
   * @return true if database is read/write
   */
  bool IsReadWrite() OVERRIDE;

  /**
   * Call backup for opened DB
   */
  bool Backup() OVERRIDE;

#ifndef QT_PORT
  /**
    * Gets connection to the SQLite database
    * @return pointer to connection
    */
  sqlite3* conn() const OVERRIDE;
#endif  // QT_PORT

 private:
#ifndef QT_PORT
  /**
   * The connection to the SQLite database
   */
  sqlite3* conn_;
#endif  // QT_PORT

  /**
   * Lock for guarding connection to database
   */
  sync_primitives::Lock conn_lock_;

  /**
   * The filename of database
   */
  std::string database_name_;

  int error_;

  /**
   * Execs query for internal using in this class
   * @param query sql query without return results
   * @return true if query was executed successfully
   */
  inline bool Exec(const std::string& query);

  friend class SQLQuery;
};

}  // namespace dbms
}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SQLITE_WRAPPER_SQL_DATABASE_IMPL_H_
