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

import QtQuick 1.1
import "Common.js" as Common
import "Async.js" as Async

Item {
    function isReady() {
        return {
            available: dataContainer.hmiTTSAvailable
        }
    }

    function speak(ttsChunks, appID) {
        // appID unused
        console.debug('enter:', ttsChunks, appID);
        if (ttsPopUp.async) {
            console.log('speak send abort');
            throw Common.Result.ABORTED;
        }

        var message = ttsChunks.map(function(str) { return str.text }).join('\n');
        ttsPopUp.activate(message);
        ttsPopUp.async = new Async.AsyncCall();
        console.debug('exit');
        return ttsPopUp.async;
    }

    function stopSpeaking() {
        console.debug("enter");
        ttsPopUp.deactivate();
        console.debug("exit");
    }

    function getLanguage() {
        return {
            language: dataContainer.hmiTTSVRLanguage
        }
    }

    function getSupportedLanguages() {
        return {
            languages: settingsContainer.sdlLanguagesList
        }
    }

    function getCapabilities() {
        return {
            capabilities: Common.SpeechCapabilities.SC_TEXT
        }
    }

    function changeRegistration(language, appID) {
        console.debug("enter:", language, appID);
        dataContainer.changeRegistrationTTSVR(language, appID);
        console.debug("exit");
    }

    function setGlobalProperties(helpPrompt, timeoutPrompt, appID) {
        console.debug("enter: " + helpPrompt + ", " + timeoutPrompt + ", " + appID)

        var newHelpPropmt = helpPrompt ?
                    helpPrompt.map(
                        function (structure) {
                            return structure.text
                        }
                     ).join(", ") :
                    dataContainer.currentApplication.helpPrompt

        var newTimeoutPrompt = timeoutPrompt ?
                    timeoutPrompt.map(
                        function (structure) {
                            return structure.text
                        }
                     ).join(", ") :
                    dataContainer.currentApplication.timeoutPrompt

        dataContainer.setApplicationProperties(dataContainer.currentApplication.appId, {
                                                   helpPrompt: newHelpPropmt,
                                                   timeoutPrompt: newTimeoutPrompt
                                               })
        console.debug("exit")
    }
}

