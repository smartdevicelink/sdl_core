/**
 * \file dbus_plugin.cpp
 * \brief DbusPlugin class source file.
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

#include "dbus_plugin.h"

#include "hmi_proxy.h"
#include "sdl_proxy.h"
#include "optional_argument.h"
#include "qml_dbus.h"
#include "dbus_controller.h"
#include "request_to_sdl.h"

#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

#include <QtCore/QString>
#include <QtDBus/QDBusConnection>

#if QT_4
#  include <QtDeclarative/QDeclarativeContext>
#  include <QtDeclarative/QDeclarativeListReference>
#  include <QtDeclarative/QDeclarativeEngine>
#elif QT_5
#  include <QtQml/QQmlContext>
#  include <QtQml/QQmlListReference>
#  include <QtQml/QQmlEngine>
#endif  // QT_VERSION
log4cxx::LoggerPtr logger_ = log4cxx::LoggerPtr(
                              log4cxx::Logger::getLogger("DBusPlugin"));

void DbusPlugin::registerTypes(const char *uri) {
  log4cxx::PropertyConfigurator::configure("log4cxx.properties");

  // @uri sdl.core.api
  qmlRegisterType<HmiProxy>(uri, 1, 0, "HMIAdapter");
  qmlRegisterType<SdlProxy>(uri, 1, 0, "SDLAdapter");

  RegisterDbusMetatypes();
  qDBusRegisterMetaType<OptionalArgument<int> >();
  qDBusRegisterMetaType<OptionalArgument<QList<int> > >();
  qDBusRegisterMetaType<OptionalArgument<QString> >();
  qDBusRegisterMetaType<OptionalArgument<QStringList> >();
  qDBusRegisterMetaType<OptionalArgument<bool> >();
  qDBusRegisterMetaType<OptionalArgument<QList<bool> > >();
  qDBusRegisterMetaType<OptionalArgument<double> >();
  qDBusRegisterMetaType<OptionalArgument<QList<double> > >();

  HmiProxy::api_adaptors_.Init(this);

  QDBusConnection::sessionBus().registerObject("/", this);
  QDBusConnection::sessionBus().registerService("com.ford.sdl.hmi");

  dbusController_ = new DBusController();
  requestToSDL_ = new RequestToSDL();

  HmiProxy::api_adaptors_.SetDBusController(dbusController_);
}

void DbusPlugin::initializeEngine(Engine *engine, const char *uri) {
    engine->rootContext()->setContextProperty("DBus", dbusController_);
    engine->rootContext()->setContextProperty("RequestToSDL", requestToSDL_);
}

#if QT_4
Q_EXPORT_PLUGIN2(DbusAdapter, DbusPlugin)
#endif  // QT_4
