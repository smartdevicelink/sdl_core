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
        console.log('enter function speak');
        if (ttsPopUp.async) {
            console.log('function speak send abort');
            throw Common.Result.ABORTED;
        }

        var message = "";
        for (var i = 0; i < ttsChunks.length; ++i)
            message += ttsChunks[i].text + "\n";
        ttsPopUp.activate(message);
        ttsPopUp.async = new Async.AsyncCall();
        console.log('exit function speak');
        return ttsPopUp.async;
    }

    function stopSpeaking() {
        console.debug("Stop speaking");
        ttsPopUp.deactivate();
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
        console.debug("ChangeRegistrationTTSVR:", language, appID);
        dataContainer.changeRegistrationTTSVR(language, appID)
    }

    function setGlobalProperties(helpPrompt, timeoutPrompt, appID) {
        dataContainer.globalProperties.helpPrompt = helpPrompt
        dataContainer.globalProperties.timeoutPrompt = timeoutPrompt
    }
}

