/**
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

import QtQuick 2.0
import ".."

Item {
  BasicCommunication {
    id: sdlBasicCommunication
  }
  function updateDeviceList(params) {
     return sdlBasicCommunication.updateDeviceList(params.deviceList)
  }

  function allowDeviceToConnect(params) {
     return sdlBasicCommunication.allowDeviceToConnect(params.device)
  }

  function updateAppList(params) {
     return sdlBasicCommunication.updateAppList(params.applications)
  }

  function activateApp(params) {
     return sdlBasicCommunication.activateApp(params.appID)
  }

  function mixingAudioSupported(params) {
     return sdlBasicCommunication.mixingAudioSupported()
  }

  function allowAllApps(params) {
     return sdlBasicCommunication.allowAllApps()
  }

  function allowApp(params) {
     return sdlBasicCommunication.allowApp(params.app, params.appPermissions)
  }

  signal onReady()
  signal onStartDeviceDiscovery()
  signal onUpdateDeviceList()
  signal onDeviceChosen(var deviceInfo)
  signal onFindApplications(var deviceInfo)
  signal onAppActivated(int appID)
  signal onAppDeactivated(int appID, int reason)
  signal onAppRegistered(var application)
  signal onAppUnregistered(int appID)
  signal onExitApplication(int appID)
  signal onExitAllApplications(int reason)
  signal playTone()
}
