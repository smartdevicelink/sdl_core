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

#ifndef SRC_COMPONENTS_POLICY_SQLITE_WRAPPER_INCLUDE_SQLITE_WRAPPER_SQL_ERROR_H_
#define SRC_COMPONENTS_POLICY_SQLITE_WRAPPER_INCLUDE_SQLITE_WRAPPER_SQL_ERROR_H_

#include <string>

namespace policy {
namespace dbms {

typedef enum Error {
  OK          = 0,    /* Successful result */
  ERROR,              /* SQL error or missing database */
  INTERNAL,           /* Internal logic error in SQLite */
  PERM,               /* Access permission denied */
  ABORT,              /* Callback routine requested an abort */
  BUSY,               /* The database file is locked */
  LOCKED,             /* A table in the database is locked */
  NOMEM,              /* A malloc() failed */
  READONLY,           /* Attempt to write a readonly database */
  INTERRUPT,          /* Operation terminated by sqlite3_interrupt()*/
  IOERR,              /* Some kind of disk I/O error occurred */
  CORRUPT,            /* The database disk image is malformed */
  NOTFOUND,           /* Unknown opcode in sqlite3_file_control() */
  FULL,               /* Insertion failed because database is full */
  CANTOPEN,           /* Unable to open the database file */
  PROTOCOL,           /* Database lock protocol error */
  EMPTY,              /* Database is empty */
  SCHEMA,             /* The database schema changed */
  TOOBIG,             /* String or BLOB exceeds size limit */
  CONSTRAINT,         /* Abort due to constraint violation */
  MISMATCH,           /* Data type mismatch */
  MISUSE,             /* Library used incorrectly */
  NOLFS,              /* Uses OS features not supported on host */
  AUTH,               /* Authorization denied */
  FORMAT,             /* Auxiliary database format error */
  RANGE,              /* 2nd parameter to sqlite3_bind out of range */
  NOTADB,             /* File opened that is not a database file */
  NOTICE,             /* Notifications from sqlite3_log() */
  WARNING,            /* Warnings from sqlite3_log() */
  ROW         = 100,  /* sqlite3_step() has another row ready */
  DONE        = 101   /* sqlite3_step() has finished executing */
} Error;

/**
 * Provides SQL database error information
 */
class SQLError {
 public:
  SQLError(Error number, const std::string& text = "");

  /**
   * Gets number of error
   * @return error number
   */
  Error number() const;

  /**
   * Gets text description of the error
   * @return text
   */
  std::string text() const;

 private:
  /**
   * Number of the error
   */
  Error number_;

  /**
   * Description of the error
   */
  mutable std::string text_;
};

}  // namespace dbms
}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_SQLITE_WRAPPER_INCLUDE_SQLITE_WRAPPER_SQL_ERROR_H_
