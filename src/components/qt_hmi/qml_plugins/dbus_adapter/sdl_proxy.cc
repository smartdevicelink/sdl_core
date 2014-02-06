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

SdlProxy::SdlProxy(Item *parent)
    : Item(parent) {

  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.BasicCommunication",
              "OnAppRegistered", this, SLOT(OnAppRegistered(Common_HMIApplication)));
  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.BasicCommunication",
              "OnAppUnregistered", this, SLOT(OnAppUnregistered(int, OptionalArgument<bool>)));
  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.BasicCommunication",
              "PlayTone", this, SIGNAL(playTone()));
  QDBusConnection::sessionBus().connect(
              "com.ford.sdl.core", "/", "com.ford.sdl.core.UI", "ShowNotification",
              this, SLOT(OnShowNotification(Common_TextFieldStruct,
                                            OptionalArgument<Common_Image>, int)));
}

void SdlProxy::OnAppRegistered(Common_HMIApplication app) {
  QVariantMap appMap;
  appMap["appId"] = QVariant::fromValue(app.appID);
  appMap["appName"] = QVariant::fromValue(app.appName);
  appMap["deviceName"] = QVariant::fromValue(app.deviceName);
  appMap["hmiDisplayLanguageDesired"] = QVariant::fromValue(app.hmiDisplayLanguageDesired);
  appMap["isMediaApplication"] = QVariant::fromValue(app.isMediaApplication);

  if (app.appType.presence) {
    QList<QVariant> appType;
    for (QList<int>::const_iterator it = app.appType.val.begin();
      it != app.appType.val.end(); ++it) {
        appType.append(QVariant::fromValue(*it));
    }
    appMap["appType"] = appType;
  }

  if (app.ngnMediaScreenAppName.presence) {
    appMap["ngnMediaScreenAppName"] = QVariant::fromValue(app.ngnMediaScreenAppName.val);
  }

  if (app.icon.presence) {
    appMap["icon"] = QVariant::fromValue(app.icon.val);
  }

  emit appRegistered(QVariant(appMap));
}

void SdlProxy::OnShowNotification(Common_TextFieldStruct text,
                                  OptionalArgument<Common_Image> image,
                                  int timeout) {
  QVariantMap txtMap;
  QVariant img;

  txtMap["fieldName"] = text.fieldName;
  txtMap["fieldText"] = text.fieldText;

  if (image.presence) {
    QVariantMap imgMap;
    imgMap["imageType"] = image.val.imageType;
    imgMap["value"] = image.val.value;
    img = QVariant(imgMap);
  }

  emit showNotification(QVariant(txtMap), img, timeout);
}

void SdlProxy::OnAppUnregistered(int appId, OptionalArgument<bool> resume) {
  QVariant resume_arg;

  if (resume.presence) {
    resume_arg = QVariant::fromValue(resume.val);
  }

  emit appUnregistered(appId, resume_arg);
}
