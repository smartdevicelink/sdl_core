/*
 * \file optional_argument.h
 * \brief OptionalArgument struct header file.
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

#ifndef SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_OPTIONAL_ARGUMENT_H_
#define SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_OPTIONAL_ARGUMENT_H_

#include "qt_version.h"
#include <QtDBus/QDBusArgument>

template <class T>
struct OptionalArgument {
  T val;
  bool presence;
  explicit OptionalArgument(const T& value) : val(value), presence(true) {}
  OptionalArgument() : presence(false) {}
};

template <class T>
inline QDBusArgument& operator<<(QDBusArgument& arg,
                                 const OptionalArgument<T>& o) {
  arg.beginStructure();
  arg << o.presence << o.val;
  arg.endStructure();
  return arg;
}

template <class T>
inline const QDBusArgument& operator>>(const QDBusArgument& arg,
                                       OptionalArgument<T>& o) {
  arg.beginStructure();
  arg >> o.presence >> o.val;
  arg.endStructure();
  return arg;
}

template <class T>
inline bool VariantToValue(const QVariant& variant, OptionalArgument<T>& o) {
  if (variant.isNull()) {
    o.presence = false;
    return true;
  }
  o.presence = true;
  return VariantToValue(variant, o.val);
}

#if QT_4
Q_DECLARE_METATYPE(QList<int>)
#endif  // QT_4

Q_DECLARE_METATYPE(OptionalArgument<int>)
Q_DECLARE_METATYPE(OptionalArgument<QString>)
Q_DECLARE_METATYPE(OptionalArgument<bool>)
Q_DECLARE_METATYPE(OptionalArgument<double>)

Q_DECLARE_METATYPE(OptionalArgument<QList<int> >)
Q_DECLARE_METATYPE(OptionalArgument<QStringList>)
Q_DECLARE_METATYPE(OptionalArgument<QList<bool> >)
Q_DECLARE_METATYPE(OptionalArgument<QList<double> >)
#endif  // SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_OPTIONAL_ARGUMENT_H_
