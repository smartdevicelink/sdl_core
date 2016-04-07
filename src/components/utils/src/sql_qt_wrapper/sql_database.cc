#include "sql_qt_wrapper/sql_database.h"

#include <QSqlError>

namespace {
const QString kDatabaseExtension = ".sqlite";
}  // namespace

namespace utils {
namespace dbms {

SQLDatabase::SQLDatabase(const std::string& database_path,
                         const std::string& connection_name)
    : database_path_((database_path + kDatabaseExtension.toStdString()).c_str())
    , connection_name_(connection_name.c_str()) {
  db_ = QSqlDatabase::addDatabase("QSQLITE", connection_name_);
}

SQLDatabase::~SQLDatabase() {
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

bool SQLDatabase::Open() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  db_.setDatabaseName(database_path_);
  return db_.open();
}

void SQLDatabase::Close() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  if (db_.isOpen()) {
    db_.close();
  }
}

bool SQLDatabase::BeginTransaction() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  return db_.transaction();
}

bool SQLDatabase::CommitTransaction() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  return db_.commit();
}

bool SQLDatabase::RollbackTransaction() {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  return db_.rollback();
}

SQLError SQLDatabase::LastError() const {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  return SQLError(db_.lastError());
}

bool SQLDatabase::HasErrors() const {
  sync_primitives::AutoLock auto_lock(conn_lock_);
  return db_.lastError().type() != QSqlError::NoError;
}

std::string SQLDatabase::get_path() const {
  return database_path_.toStdString();
}

bool SQLDatabase::IsReadWrite() {
  return true;
}

SQLDatabase::operator QSqlDatabase() const {
  return db_;
}

bool SQLDatabase::Backup() {
  return true;
}

}  // namespace dbms
}  // namespace utils
