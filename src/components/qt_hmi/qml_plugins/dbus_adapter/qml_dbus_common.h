/**
 * \file qml_dbus_common.h
 * \brief Contain utilities for DBus plugin.
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

#ifndef SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_QML_DBUS_COMMON_H_
#define SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_QML_DBUS_COMMON_H_

#include <QtDBus/QDBusContext>
#include "optional_argument.h"
#include "stream_qvariant.h"
#include "metatype.h"

enum ErrorCode {
  Success = 0,
  UnsupportedRequest,
  UnsupportedResource,
  Disallowed,
  Rejected,
  Aborted,
  Ignored,
  Retry,
  InUse,
  DataNotAvailable,
  TimedOut,
  InvalidData,
  CharLimitExceeded,
  InvalidId,
  DuplicateName,
  ApplicationNotRegistered,
  WrongLanguage,
  OutOfMemory,
  TooManyPendingRequests,
  NoAppsRegistered,
  NoDevicesConnected,
  Warnings,
  GenericError,
  UserDisallowed
};

inline void RaiseDbusError(QObject* adaptor, int code, const QString& message) {
  QDBusContext* context = dynamic_cast<QDBusContext*>(adaptor->parent());
  if (context) {
    context->sendErrorReply(QString::number(static_cast<int>(code)),
                            message);
  }
}

inline void RaiseDbusError(QObject* adaptor, int code) {
  RaiseDbusError(adaptor, code, "");
}

template<typename T>
inline void PutArgToMap(QVariantMap& map, const char* name, const T& v) {
  map.insert(name, QVariant::fromValue(v));
}

inline bool GetArgFromMap(const QVariantMap& map, const char* name, int& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Int) return false;
  v = it->toInt();
  return true;
}

inline bool GetArgFromMap(const QVariantMap& map, const char* name, QString& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::String) return false;
  v = it->toString();
  return true;
}

inline bool GetArgFromMap(const QVariantMap& map, const char* name, bool& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (it->type() != QVariant::Bool) return false;
  v = it->toBool();
  return true;
}

inline bool isNumber(QVariant v) {
  QVariant::Type t = v.type();
  return (t == QVariant::Double) || (t == QVariant::Int) ||
         (t == QVariant::UInt)   || (t == QVariant::LongLong) ||
         (t == QVariant::ULongLong);
}

inline bool GetArgFromMap(const QVariantMap& map, const char* name, double& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  if (!isNumber(*it)) return false;
  v = it->toDouble();
  return true;
}

inline bool VariantToValue(const QVariant& variant, int& v) {
    if (variant.type() != QVariant::Int) return false;
    v = variant.toInt();
    return true;
}

inline bool VariantToValue(const QVariant& variant, QString& v) {
    if (variant.type() != QVariant::String) return false;
    v = variant.toString();
    return true;
}

inline bool VariantToValue(const QVariant& variant, bool& v) {
    if (variant.type() != QVariant::Bool) return false;
    v = variant.toBool();
    return true;
}

inline bool VariantToValue(const QVariant& variant, double& v) {
    if (variant.type() != QVariant::Double) return false;
    v = variant.toDouble();
    return true;
}

inline bool VariantToValue(const QVariant& variant, QStringList& v) {
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    if (i->type() != QVariant::String) return false;
    v.append(i->toString());
  }
  return true;
}

template<typename T>
bool VariantToValue(const QVariant& variant, QList<T>& v) {
    if (variant.type() != QVariant::List) return false;
    QList<T> spare;
    QList<QVariant> list = variant.toList();
    for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
        QVariant::Type type = i->type();
// Although this function is declared as returning QVariant::Type(obsolete),
// the return value should be interpreted as QMetaType::Type.
// (http://qt-project.org/doc/qt-5.0/qtcore/qvariant.html#type)
        QMetaType::Type type_casted = static_cast<QMetaType::Type>(type);
        if (type_casted != metatype<T>()) {
          return false;
        }
        spare.append(i->value<T>());
    }
    v.swap(spare);
    return true;
}

template<typename T>
inline QVariant ValueToVariant(const T& v) {
    return QVariant::fromValue(v);
}

template<typename T>
inline QVariant ValueToVariant(const QList<T>& v) {
  QList<QVariant> list;
  for (typename QList<T>::const_iterator i = v.begin(); i != v.end(); ++i)
    list.append(ValueToVariant(*i));
  return QVariant::fromValue(list);
}

template<typename T>
inline void PutArgToMap(QVariantMap& map, const char* name, const QList<T>& v) {
  QList<QVariant> list;
  for (typename QList<T>::const_iterator i = v.begin(); i != v.end(); ++i)
    list.append(ValueToVariant(*i));
  map.insert(name, QVariant::fromValue(list));
}

template<typename T>
inline bool GetArgFromMap(const QVariantMap& map, const char* name, QList<T>& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it) return false;
  const QVariant& variant = *it;
  if (variant.type() != QVariant::List) return false;
  QList<QVariant> list = variant.toList();
  for (QList<QVariant>::const_iterator i = list.begin(); i != list.end(); ++i) {
    T t;
    bool ok = VariantToValue(*i, t);
    if (!ok) return false;
    v.append(t);
  }
  return true;
}

template<typename T>
inline void PutArgToMap(QVariantMap& map, const char* name,
                        const OptionalArgument<T>& v) {
  if (v.presence)
    map.insert(name, ValueToVariant(v.val));
}

template<typename T>
inline bool GetArgFromMap(const QVariantMap& map, const char* name,
                          OptionalArgument<T>& v) {
  QVariantMap::const_iterator it = map.find(name);
  if (map.end() == it || !it->isValid()) {
    v.presence = false;
    return true;
  }
  v.presence = true;
  return GetArgFromMap(map, name, v.val);
}

#endif  // SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_QML_DBUS_COMMON_H_
