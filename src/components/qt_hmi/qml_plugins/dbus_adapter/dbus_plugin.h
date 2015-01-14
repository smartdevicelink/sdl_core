/**
 * \file dbus_plugin.h
 * \brief DbusPlugin class header file.
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

#ifndef SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_DBUS_PLUGIN_H_
#define SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_DBUS_PLUGIN_H_

#include "qt_version.h"

#if QT_4
#  include <QtDeclarative/QDeclarativeExtensionPlugin>
typedef QDeclarativeExtensionPlugin ExtensionPlugin;
typedef QDeclarativeEngine Engine;
#elif QT_5
#  include <QtQml/QQmlExtensionPlugin>
typedef QQmlExtensionPlugin ExtensionPlugin;
typedef QQmlEngine Engine;
#endif  // QT_VERSION
#include <QtDBus/QDBusContext>

class DBusController;
class RequestToSDL;

class DbusPlugin : public ExtensionPlugin, public QDBusContext {
  Q_OBJECT

#if QT_5
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
#endif  // QT_5

  DBusController *dbusController_;
  RequestToSDL *requestToSDL_;

 public:
  void registerTypes(const char *uri);
  void initializeEngine(Engine *engine, const char *uri);
};

#endif  // SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_DBUS_PLUGIN_H_

