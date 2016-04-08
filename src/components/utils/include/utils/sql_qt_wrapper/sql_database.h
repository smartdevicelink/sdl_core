#ifndef SRC_COMPONENTS_POLICY_SQLITE_WRAPPER_INCLUDE_SQL_QT_WRAPPER_SQL_DATABASE
#define SRC_COMPONENTS_POLICY_SQLITE_WRAPPER_INCLUDE_SQL_QT_WRAPPER_SQL_DATABASE

#include <string>

#include <QtSql/QSqlDatabase>

#include "utils/lock.h"
#include "utils/sql_qt_wrapper/sql_error.h"

namespace utils {
namespace dbms {

/**
 * Represents a connection to a database.
 */
class SQLDatabase {
 public:
  SQLDatabase(const std::string& database_path,
              const std::string& connection_name);
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
   * @brief HasErrors Indicate the status of the last executed operation.
   *
   * @return true in case last operation has any errors, false otherwise.
   */
  bool HasErrors() const;

  /**
   * @brief get_path databse location path.
   *
   * @return the path to the database location
   */
  std::string get_path() const;

  /**
   * Checks if database is read/write
   * @return true if database is read/write
   */
  bool IsReadWrite();

  /**
   * Call backup for opened DB
   */
  bool Backup();

  operator QSqlDatabase() const;

 private:
  QSqlDatabase db_;

  /**
   * The filename of database
   */
  const QString database_path_;

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

#endif  // SRC_COMPONENTS_POLICY_SQLITE_WRAPPER_INCLUDE_SQL_QT_WRAPPER_SQL_DATABASE
