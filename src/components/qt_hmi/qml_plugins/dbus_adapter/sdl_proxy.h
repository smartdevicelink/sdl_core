/**
 * \file sdlproxy.h
 * \brief SdlProxy class header file.
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

#ifndef SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_SDL_PROXY_H_
#define SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_SDL_PROXY_H_

#include "qml_dbus.h"
#include "qt_version.h"

#include <QtDBus/QDBusInterface>
#if QT_4
# include <QtDeclarative/QDeclarativeItem>
typedef QDeclarativeItem Item;
#elif QT_5
#  include <QtQuick/QQuickItem>
typedef QQuickItem Item;
#endif  // QT_VERSION

class SdlProxy: public Item {
  Q_OBJECT
  Q_DISABLE_COPY(SdlProxy)

 public:
  explicit SdlProxy(Item* parent = 0);

 private:
  QDBusInterface *sdlBasicCommunicationInterface;

 signals:
//! BasicCommunication
  void resumeAudioSource(int appId);
  void fileRemoved(QString fileName, int fileType, int appId);
  void appRegistered(QVariant application, QVariant ttsName, QVariant vrSynonyms,
                     QVariant resumeVrGrammars, QVariant priority);
  void appUnregistered(int appId);
  void playTone();
  void sdlClose();
  void putFile(QVariant offset, QVariant length, QVariant fileSize,
               QString fileName, QString syncFileName, int fileType,
               QVariant persistentFile);
//! BasicCommunication

//! UI
  void recordStart(int appId);
//! UI

//! SDL
  void appPermissionChanged(int appId, QVariant isAppPermissionsRevoked,
                            QVariant appRevokedPermissions, QVariant appRevoked,
                            QVariant appPermissionsConsentNeeded,
                            QVariant appUnauthorized, QVariant priority);
  void sdlConsentNeeded(QVariant device);
  void statusUpdate(int status);
//! SDL

 private slots:
//! BasicCommunication
  void OnResumeAudioSource(int appId);
  void OnFileRemoved(QString fileName, int fileType, int appId);
  void OnAppRegistered(Common_HMIApplication application,
                       OptionalArgument<QList<Common_TTSChunk> > ttsName,
                       OptionalArgument<QList<QString> > vrSynonyms,
                       OptionalArgument<bool> resumeVrGrammars,
                       OptionalArgument<int> priority);
  void OnAppUnregistered(int appId);
  void OnPlayTone();
  void OnSDLClose();
  void OnPutFile(OptionalArgument<int> offset, OptionalArgument<int> length,
                 OptionalArgument<int> fileSize,
                 QString fileName, QString syncFileName, int fileType,
                 OptionalArgument<bool> persistentFile);
//! BasicCommunication

//! UI
  void OnRecordStart(int appId);
//! UI

//! SDL
  void OnAppPermissionChanged(int appId, OptionalArgument<bool> isAppPermissionsRevoked,
                              OptionalArgument<QList<Common_PermissionItem> > appRevokedPermissions,
                              OptionalArgument<bool> appRevoked,
                              OptionalArgument<bool> appPermissionsConsentNeeded,
                              OptionalArgument<bool> appUnauthorized,
                              OptionalArgument<int> priority);
  void OnSDLConsentNeeded(Common_DeviceInfo device);
  void OnStatusUpdate(int status);
//! SDL
};

QML_DECLARE_TYPE(SdlProxy)

#endif  // SRC_COMPONENTS_QT_HMI_QML_PLUGINS_DBUS_ADAPTER_SDL_PROXY_H_
