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
import "../models/Constants.js" as Constants
import "Async.js" as Async

Item {
    property string speakText
    property var async

    signal onLanguageChange (int language)

    function isReady () {
        return {
            available: dataContainer.hmiTTSAvailable
        }
    }

    function speak (ttsChunks, appID) {
        console.log('enter function speak')
        var str = ''
        for (var i in ttsChunks) {
            str = str + ttsChunks[i].text + "\n"
        }
        timer.stop()
        if (async) {
            console.log("function speak send abort")
            DBus.sendError(async, Common.Result.ABORTED)
        }
        async = new Async.AsyncCall()
        ttsModel.speakedText = str
        timer.start()
        console.log('exit function speak')
        return async
    }

    function stopSpeaking () {
    }

    function getLanguage () {
        return {
            language: dataContainer.hmiTTSVRLanguage
        }
    }

    function getSupportedLanguages () {
        return {
            languages: settingsContainer.sdlLanguagesList
        }
    }

    function changeRegistration (language) {
        dataContainer.changeRegistrationTTSVR(language)
    }

    function setGlobalProperties (helpPrompt, timeoutPrompt, appID) {
        dataContainer.globalProperties.helpPrompt = helpPrompt
        dataContainer.globalProperties.timeoutPrompt = timeoutPrompt
    }

    Timer {
        id: timer
        interval: Constants.ttsSpeakTime
        onTriggered: {
            console.log("function speak timer triggered enter")
            if (async) {
                console.log("function speak send OK")
                DBus.sendReply(async, {})
            }
            async = undefined
            ttsModel.speakedText = ''
            console.log("function speak timer triggered exit")
        }
    }
}

