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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_SQL_DATABASE_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_SQL_DATABASE_H_

#include <string>

struct sqlite3;

namespace utils {
namespace dbms {

class SQLQuery;
class SQLError;

/**
 * Represents a connection to a database.
 */
class SQLDatabase {
 public:
  virtual ~SQLDatabase() {}
  /**
   * Opens connection to the temporary in-memory database
   * @return true if successfully
   */
  virtual bool Open() = 0;

  /**
   * Closes connection to the database
   */
  virtual void Close() = 0;

  /**
   * Begins a transaction on the database
   * @return true if successfully
   */
  virtual bool BeginTransaction() = 0;

  /**
   * Commits a transaction to the database
   * @return true if successfully
   */
  virtual bool CommitTransaction() = 0;

  /**
   * Rolls back a transaction on the database
   * @return true if successfully
   */
  virtual bool RollbackTransaction() = 0;

  /**
   * Gets information about the last error that occurred on the database
   * @return last error
   */
  virtual SQLError LastError() const = 0;

  /**
   * @brief get_path databse location path.
   *
   * @return the path to the database location
   */
  virtual std::string get_path() const = 0;

#ifndef __QNX__
  /**
   * @brief HasErrors Indicate the status of the last executed operation.
   *
   * @return true in case last operation has any errors, false otherwise.
   */
  virtual bool HasErrors() const = 0;

  /**
   * Sets path to database
   * If the database is already opened then need reopen it
   */
  virtual void set_path(const std::string& path) = 0;

  /**
   * Checks if database is read/write
   * @return true if database is read/write
   */
  virtual bool IsReadWrite() = 0;

#endif  // __QNX__

  /**
   * Call backup for opened DB
   */
  virtual bool Backup() = 0;

/**
 * Gets connection to the SQLite database
 * @return pointer to connection
 */
#ifdef __QNX__
  virtual qdb_hdl_t* conn() const = 0;
#elif defined(OS_LINUX) || \
    (defined(OS_WINDOWS) && !defined(QT_PORT))  // __QNX__
  virtual sqlite3* conn() const = 0;
#endif                                          // __QNX__
};

}  // namespace dbms
}  // namespace utils

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_SQL_DATABASE_H_
