/**
 * \file sdlproxy.cpp
 * \brief SdlProxy class source file.
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

#include "sdl_proxy.h"

#include "utils/logger.h"
#ifdef ENABLE_LOG
extern log4cxx::LoggerPtr logger_;
#endif // ENABLE_LOG

SdlProxy::SdlProxy(Item *parent)
    : Item(parent) {

  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.BasicCommunication",
              "OnResumeAudioSource", this, SLOT(OnResumeAudioSource(int)));
  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.BasicCommunication",
              "OnFileRemoved", this, SLOT(OnFileRemoved(QString, int, int)));
  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.BasicCommunication",
              "OnAppRegistered", this, SLOT(OnAppRegistered(Common_HMIApplication,
                OptionalArgument<QList<Common_TTSChunk> >,
                OptionalArgument<QStringList>,
                OptionalArgument<bool>, OptionalArgument<int>)));
  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.BasicCommunication",
              "OnAppUnregistered", this, SLOT(OnAppUnregistered(int)));
  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.BasicCommunication",
              "PlayTone", this, SLOT(OnPlayTone()));
  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.BasicCommunication",
              "OnSDLClose", this, SLOT(OnSDLClose()));
  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.BasicCommunication",
              "OnPutFile", this, SLOT(OnPutFile(OptionalArgument<int>,
                OptionalArgument<int>, OptionalArgument<int>,
                QString, QString, int, OptionalArgument<bool>)));

  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.UI", "OnRecordStart",
              this, SLOT(OnRecordStart(int)));

  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.SDL", "OnAppPermissionChanged",
              this, SLOT(OnAppPermissionChanged()));
  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.SDL", "OnSDLConsentNeeded",
              this, SLOT(OnSDLConsentNeeded()));
  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.SDL", "OnStatusUpdate",
              this, SLOT(OnStatusUpdate(int)));
}

void SdlProxy::OnResumeAudioSource(int appId) {
    LOG4CXX_TRACE(logger_, "SIGNAL from SDL");
    emit resumeAudioSource(appId);
}

void SdlProxy::OnFileRemoved(QString fileName, int fileType, int appId) {
    LOG4CXX_TRACE(logger_, "SIGNAL from SDL");
    emit fileRemoved(fileName, fileType, appId);
}

void SdlProxy::OnAppRegistered(Common_HMIApplication application,
                               OptionalArgument<QList<Common_TTSChunk> > ttsName,
                               OptionalArgument<QStringList> vrSynonyms,
                               OptionalArgument<bool> resumeVrGrammars,
                               OptionalArgument<int> priority) {
  LOG4CXX_TRACE(logger_, "SIGNAL from SDL");
  QVariantMap appMap;
  appMap["appId"] = QVariant::fromValue(application.appID);
  appMap["appName"] = QVariant::fromValue(application.appName);
  appMap["deviceName"] = QVariant::fromValue(application.deviceName);
  appMap["hmiDisplayLanguageDesired"] = QVariant::fromValue(application.hmiDisplayLanguageDesired);
  appMap["isMediaApplication"] = QVariant::fromValue(application.isMediaApplication);

  if (application.appType.presence) {
    QList<QVariant> appType;
    for (QList<int>::const_iterator it = application.appType.val.begin();
      it != application.appType.val.end(); ++it) {
      appType.append(QVariant::fromValue(*it));
    }
    appMap["appType"] = appType;
  }

  if (application.ngnMediaScreenAppName.presence) {
    appMap["ngnMediaScreenAppName"] = QVariant::fromValue(application.ngnMediaScreenAppName.val);
  }

  if (application.icon.presence) {
    appMap["icon"] = QVariant::fromValue(application.icon.val);
  }

  QVariant ttsNameArray;
  if (ttsName.presence) {
    QList<QVariantMap> ttsChunks;
    for (QList<Common_TTSChunk>::const_iterator it = ttsName.val.begin();
      it != ttsName.val.end(); ++it) {
      QVariantMap ttsChunkMap;
      ttsChunkMap["text"] = QVariant::fromValue(it->text);
      ttsChunkMap["type"] = QVariant::fromValue(it->type);
      ttsChunks.append(ttsChunkMap);
    }
    ttsNameArray = QVariant::fromValue(ttsChunks);
  }

  QVariant vrSynonymsArray;
  if (ttsName.presence) {
    QList<QVariant> names;
    for (QStringList::const_iterator it = vrSynonyms.val.begin();
      it != vrSynonyms.val.end(); ++it) {
      names.append(QVariant::fromValue(*it));
    }
    vrSynonymsArray = names;
  }

  QVariant resumeVrGrammarsValue;
  if (resumeVrGrammars.presence) {
    resumeVrGrammarsValue = QVariant::fromValue(resumeVrGrammars.val);
  }

  QVariant priopityValue;
  if (priority.presence) {
    priopityValue = QVariant::fromValue(priority.val);
  }

  emit appRegistered(QVariant(appMap), ttsNameArray, vrSynonymsArray,
                     resumeVrGrammarsValue, priopityValue);
}

void SdlProxy::OnAppUnregistered(int appId) {
  LOG4CXX_TRACE(logger_, "SIGNAL from SDL");
  emit appUnregistered(appId);
}

void SdlProxy::OnPlayTone() {
  LOG4CXX_TRACE(logger_, "SIGNAL from SDL");
  emit playTone();
}

void SdlProxy::OnSDLClose() {
  LOG4CXX_TRACE(logger_, "SIGNAL from SDL");
  emit sdlClose();
}

void SdlProxy::OnPutFile(OptionalArgument<int> offset, OptionalArgument<int> length,
                         OptionalArgument<int> fileSize, QString fileName,
                         QString syncFileName, int fileType,
                         OptionalArgument<bool> persistentFile) {
  LOG4CXX_TRACE(logger_, "SIGNAL from SDL");

  QVariant offsetValue;
  if (offset.presence) {
    offsetValue = QVariant::fromValue(offset.val);
  }

  QVariant lengthValue;
  if (length.presence) {
    lengthValue = QVariant::fromValue(length.val);
  }

  QVariant fileSizeValue;
  if (fileSize.presence) {
    fileSizeValue = QVariant::fromValue(fileSize.val);
  }

  QVariant persistentFileValue;
  if (persistentFile.presence) {
    persistentFileValue = QVariant::fromValue(persistentFile.val);
  }

  emit putFile(offsetValue, lengthValue, fileSizeValue, fileName,
               syncFileName, fileType, persistentFileValue);
}

void SdlProxy::OnRecordStart(int appId) {
  LOG4CXX_TRACE(logger_, "SIGNAL from SDL");
  emit recordStart(appId);
}

void SdlProxy::OnAppPermissionChanged(int appId,
                                      OptionalArgument<bool> isAppPermissionsRevoked,
                                      OptionalArgument<QList<Common_PermissionItem> > appRevokedPermissions,
                                      OptionalArgument<bool> appRevoked,
                                      OptionalArgument<bool> appPermissionsConsentNeeded,
                                      OptionalArgument<bool> appUnauthorized,
                                      OptionalArgument<int> priority) {
  LOG4CXX_TRACE(logger_, "SIGNAL from SDL");
  QVariant isAppPermissionsRevokedValue;
  if (isAppPermissionsRevoked.presence) {
    isAppPermissionsRevokedValue = QVariant::fromValue(isAppPermissionsRevoked.val);
  }

  QVariant appRevokedPermissionsArray;
  if (appRevokedPermissions.presence) {
    QList<QVariantMap> permissions;
    for (QList<Common_PermissionItem>::const_iterator it = appRevokedPermissions.val.begin();
      it != appRevokedPermissions.val.end(); ++it) {
      QVariantMap permissionMap;
      permissionMap["name"] = QVariant::fromValue(it->name);
      permissionMap["id"] = QVariant::fromValue(it->id);
      if (it->allowed.presence) {
        permissionMap["allowed"] = QVariant::fromValue(it->allowed.val);
      }
      permissions.append(permissionMap);
    }
    appRevokedPermissionsArray = QVariant::fromValue(permissions);
  }

  QVariant appRevokedValue;
  if (appRevoked.presence) {
    appRevokedValue = QVariant::fromValue(appRevoked.val);
  }

  QVariant appPermissionsConsentNeededValue;
  if (appPermissionsConsentNeeded.presence) {
    appPermissionsConsentNeededValue = QVariant::fromValue(appPermissionsConsentNeeded.val);
  }

  QVariant appUnauthorizedValue;
  if (appUnauthorized.presence) {
    appUnauthorizedValue = QVariant::fromValue(appUnauthorized.val);
  }

  QVariant priorityValue;
  if (priority.presence) {
    priorityValue = QVariant::fromValue(priority.val);
  }

  emit appPermissionChanged(appId, isAppPermissionsRevokedValue,
                            appRevokedPermissionsArray, appRevokedValue,
                            appPermissionsConsentNeededValue,
                            appUnauthorizedValue, priorityValue);
}

void SdlProxy::OnSDLConsentNeeded(Common_DeviceInfo device) {
  LOG4CXX_TRACE(logger_, "SIGNAL from SDL");
  QVariantMap deviceMap;
  deviceMap["name"] = QVariant::fromValue(device.name);
  deviceMap["id"] = QVariant::fromValue(device.id);

  emit sdlConsentNeeded(QVariant(deviceMap));
}

void SdlProxy::OnStatusUpdate(int status) {
  LOG4CXX_TRACE(logger_, "SIGNAL from SDL");
  emit statusUpdate(status);
}
