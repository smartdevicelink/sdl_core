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
  UI {
    id: sdlUI
  }
  function alert(params) {
     return sdlUI.alert(params.alertStrings, params.duration, params.softButtons, params.appID)
  }

  function show(params) {
     return sdlUI.show(params.showStrings, params.alignment, params.graphic, params.softButtons, params.customPresets, params.appID)
  }

  function addCommand(params) {
     return sdlUI.addCommand(params.cmdID, params.menuParams, params.cmdIcon, params.appID)
  }

  function deleteCommand(params) {
     return sdlUI.deleteCommand(params.cmdID, params.appID)
  }

  function addSubMenu(params) {
     return sdlUI.addSubMenu(params.menuID, params.menuParams, params.appID)
  }

  function deleteSubMenu(params) {
     return sdlUI.deleteSubMenu(params.menuID, params.appID)
  }

  function performInteraction(params) {
     return sdlUI.performInteraction(params.initialText, params.choiceSet, params.vrHelp, params.timeout, params.appID)
  }

  function setMediaClockTimer(params) {
     return sdlUI.setMediaClockTimer(params.startTime, params.updateMode, params.appID)
  }

  function setGlobalProperties(params) {
     return sdlUI.setGlobalProperties(params.vrHelpTitle, params.vrHelp, params.appID)
  }

  function showVrHelp(params) {
     return sdlUI.showVrHelp(params.vrHelpTitle, params.vrHelp, params.appID)
  }

  function getCapabilities(params) {
     return sdlUI.getCapabilities()
  }

  function changeRegistration(params) {
     return sdlUI.changeRegistration(params.language, params.appID)
  }

  function getSupportedLanguages(params) {
     return sdlUI.getSupportedLanguages()
  }

  function getLanguage(params) {
     return sdlUI.getLanguage()
  }

  function setAppIcon(params) {
     return sdlUI.setAppIcon(params.syncFileName, params.appID)
  }

  function slider(params) {
     return sdlUI.slider(params.numTicks, params.position, params.sliderHeader, params.sliderFooter, params.timeout, params.appID)
  }

  function scrollableMessage(params) {
     return sdlUI.scrollableMessage(params.messageText, params.timeout, params.softButtons, params.appID)
  }

  function performAudioPassThru(params) {
     return sdlUI.performAudioPassThru(params.audioPassThruDisplayTexts, params.maxDuration)
  }

  function endAudioPassThru(params) {
     return sdlUI.endAudioPassThru()
  }

  function isReady(params) {
     return sdlUI.isReady()
  }

  function closePopUp(params) {
     return sdlUI.closePopUp()
  }

  signal showNotification(var text, var icon, int timeout)
  signal onCommand(int cmdID, int appID)
  signal onSystemContext(int systemContext)
  signal onLanguageChange(int language)
  signal onDriverDistraction(int state)
}
