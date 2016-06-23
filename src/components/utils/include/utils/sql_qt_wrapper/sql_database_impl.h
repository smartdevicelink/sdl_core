#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SQL_QT_WRAPPER_SQL_DATABASE_IMPL_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SQL_QT_WRAPPER_SQL_DATABASE_IMPL_H_

#include <string>

#include <QtSql/QSqlDatabase>

#include "utils/lock.h"
#include "utils/sql_qt_wrapper/sql_error.h"
#include "utils/sql_database.h"

namespace utils {
namespace dbms {

/**
 * Represents a connection to a database.
 */
class SQLDatabaseImpl : public SQLDatabase {
 public:
  SQLDatabaseImpl();
  SQLDatabaseImpl(const std::string& database_path,
                  const std::string& connection_name);
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

  /**
   * Sets path to database
   * If the database is already opened then need reopen it
   */
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

  operator QSqlDatabase() const;

 private:
  QSqlDatabase db_;

  /**
   * The filename of database
   */
  QString database_path_;

  /**
   * The database connection name
   */
  const QString connection_name_;

  /**
   * Lock for guarding connection to database
   */
  mutable sync_primitives::Lock conn_lock_;
};

}  // namespace dbms
}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SQL_QT_WRAPPER_SQL_DATABASE_IMPL_H_
