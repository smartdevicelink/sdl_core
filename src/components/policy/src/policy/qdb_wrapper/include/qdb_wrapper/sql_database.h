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

#ifndef SRC_COMPONENTS_POLICY_QDB_WRAPPER_INCLUDE_QDB_WRAPPER_SQL_DATABASE_H_
#define SRC_COMPONENTS_POLICY_QDB_WRAPPER_INCLUDE_QDB_WRAPPER_SQL_DATABASE_H_

#include <qdb/qdb.h>
#include <string>
#include "qdb_wrapper/sql_error.h"
#include "utils/lock.h"

namespace policy {
namespace dbms {

class SQLQuery;

/**
 * Represents a connection to a database.
 */
class SQLDatabase {
 public:
  explicit SQLDatabase(const std::string& db_name);
  ~SQLDatabase();

  /**
   * Opens connection to the temporary in-memory database
   * @return true if successfully
   */
  bool Open();

  /**
   * Closes connection to the database
   */
  void Close();

  /**
   * Begins a transaction on the database
   * @return true if successfully
   */
  bool BeginTransaction();

  /**
   * Commits a transaction to the database
   * @return true if successfully
   */
  bool CommitTransaction();

  /**
   * Rolls back a transaction on the database
   * @return true if successfully
   */
  bool RollbackTransaction();

  /**
   * Gets information about the last error that occurred on the database
   * @return last error
   */
  SQLError LastError() const;

  /**
   * Call backup for opened DB
   */
  bool Backup();
 protected:
  /**
   * Gets connection to the SQLite database
   * @return pointer to connection
   */
  qdb_hdl_t* conn() const;

 private:
  /**
   * The connection to the SQLite database
   */
  qdb_hdl_t* conn_;

  /**
   * Lock for guarding connection to database
   */
  sync_primitives::Lock conn_lock_;

  /**
   * The database name
   */
  std::string db_name_;

  /**
   * The last error that occurred on the database
   */
  Error error_;

  /**
   * Execs query for internal using in this class
   * @param query sql query without return results
   * @return true if query was executed successfully
   */
  inline bool Exec(const std::string& query);

  friend class SQLQuery;
};

}  // namespace dbms
}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_QDB_WRAPPER_INCLUDE_QDB_WRAPPER_SQL_DATABASE_H_
