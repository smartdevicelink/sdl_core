#include "sql_qt_wrapper/sql_database_impl.h"

#include <QSqlError>

namespace {
const QString kDatabaseExtension = ".sqlite";
}  // namespace

namespace utils {
namespace dbms {

SQLDatabaseImpl::SQLDatabaseImpl() : database_path_() {}

SQLDatabaseImpl::SQLDatabaseImpl(const std::string& database_path,
                                 const std::string& connection_name)
    : database_path_((database_path + kDatabaseExtension.toStdString()).c_str())
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
