/**
 * @file log4cxx_plugin.h
 * @brief Log4cxxPlugin class header file.
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

#ifndef SRC_COMPONENTS_QT_HMI_QML_PLUGINS_LOG4CXX_LOG4CXX_PLUGIN_H_
#define SRC_COMPONENTS_QT_HMI_QML_PLUGINS_LOG4CXX_LOG4CXX_PLUGIN_H_

#include <qglobal.h>

#define QT_4 ((QT_VERSION >= QT_VERSION_CHECK(4, 8, 0)) && \
  (QT_VERSION < QT_VERSION_CHECK(5, 0, 0)))

#define QT_5 ((QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)) && \
  (QT_VERSION < QT_VERSION_CHECK(6, 0, 0)))

#if QT_4
#  include <QtDeclarative/qdeclarative.h>
#  include <QtDeclarative/QDeclarativeExtensionPlugin>
typedef QDeclarativeExtensionPlugin ExtensionPlugin;
#elif QT_5
#  include <QtQml/qqml.h>
#  include <QtQml/QQmlExtensionPlugin>
typedef QQmlExtensionPlugin ExtensionPlugin;
#endif  // QT_VERSION

class Logger : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(Logger)

 public:
  explicit Logger(QObject *parent = 0) : QObject(parent) {}
};

class Log4cxxPlugin : public ExtensionPlugin {
  Q_OBJECT
#if QT_5
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
#endif  // QT_5

 public:
  void registerTypes(const char *uri);
};

QML_DECLARE_TYPE(Logger)

#endif  // SRC_COMPONENTS_QT_HMI_QML_PLUGINS_LOG4CXX_LOG4CXX_PLUGIN_H_

