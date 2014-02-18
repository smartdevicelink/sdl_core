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
    function ttsChunksToString(ttsChunks){
        return ttsChunks.map(function(str) { return str.text }).join('\n')
    }

    function isReady() {
        console.log("Message Received - {method: 'TTS.IsReady'}")
        return {
            available: dataContainer.hmiTTSAvailable
        }
    }

    function speak(ttsChunks, appID) {
        // appID unused
        console.debug('enter:', ttsChunks, appID);
        var ttsChunksLog = "";
        if (ttsChunks) {
            for (var i = 0; i < ttsChunks.length; i++) {
                ttsChunksLog += "{type: " + ttsChunks[i].type + ", " +
                        "text: '" + ttsChunks[i].text + "'}, ";
            }
        }
        console.log("Message Received - {method: 'TTS.Speak', params:{ " +
                    "ttsChunks: [" + ttsChunksLog + "], " +
                    "appID: " + appID + "', " +
                    "}}")
        if (ttsPopUp.async) {
            console.log('speak send abort');
            throw Common.Result.ABORTED;
        }
        var message = ttsChunksToString(ttsChunks);
        ttsPopUp.activate(message);
        ttsPopUp.async = new Async.AsyncCall();
        console.debug('exit');
        return ttsPopUp.async;
    }

    function stopSpeaking() {
        console.debug("enter");
        console.log("Message Received - {method: 'TTS.StopSpeaking'}")
        ttsPopUp.deactivate();
        console.debug("exit");
    }

    function getLanguage() {
        console.log("Message Received - {method: 'TTS.GetLanguage'}")
        return {
            language: dataContainer.hmiTTSVRLanguage
        }
    }

    function getSupportedLanguages() {
        console.log("Message Received - {method: 'TTS.GetSupportedLanguages'}")
        return {
            languages: settingsContainer.sdlLanguagesList
        }
    }

    function getCapabilities() {
        console.log("Message Received - {method: 'TTS.GetCapabilities'}")
        return {
            speechCapabilities: [
                Common.SpeechCapabilities.SC_TEXT,
                Common.SpeechCapabilities.PRE_RECORDED
            ],
            prerecordedSpeechCapabilities: [
                Common.PrerecordedSpeech.HELP_JINGLE,
                Common.PrerecordedSpeech.INITIAL_JINGLE,
                Common.PrerecordedSpeech.LISTEN_JINGLE,
                Common.PrerecordedSpeech.POSITIVE_JINGLE,
                Common.PrerecordedSpeech.NEGATIVE_JINGLE
            ]
        }
    }

    function performInteraction(helpPrompt, initialPrompt, timeoutPrompt, timeout) {
        console.debug("enter");
        var helpttsChunksLog = "",
            initialttsChunkLog = "",
            timeoutttsChunkLog = "";

        if (helpPrompt) {
            for (var i = 0; i < helpPrompt.length; i++) {
                helpttsChunksLog += "{type: " + helpPrompt[i].type + ", " +
                        "text: '" + helpPrompt[i].text + "'}, ";
            }
        }
        if (initialPrompt) {
            for (var i = 0; i < initialPrompt.length; i++) {
                initialttsChunkLog += "{type: " + initialPrompt[i].type + ", " +
                        "text: '" + initialPrompt[i].text + "'}, ";
            }
        }
        if (timeoutPrompt) {
            for (var i = 0; i < timeoutPrompt.length; i++) {
                timeoutttsChunkLog += "{type: " + timeoutPrompt[i].type + ", " +
                        "text: '" + timeoutPrompt[i].text + "'}, ";
            }
        }
        console.log("Message Received - {method: 'TTS.PerformInteraction', params:{ " +
                    "helpPrompt: [" + helpttsChunksLog + "], " +
                    "initialPrompt: [" + initialttsChunkLog + "], " +
                    "timeoutPrompt: [" + timeoutttsChunkLog + "], " +
                    "timeout: " + timeout +
                    "}}")
        ttsPopUp.performInteraction(ttsChunksToString(helpPrompt),
                                    ttsChunksToString(initialPrompt),
                                    ttsChunksToString(timeoutPrompt),
                                    timeout)
        console.debug("exit");
    }

    function changeRegistration(language, appID) {
        console.debug("enter:", language, appID);
        console.log("Message Received - {method: 'TTS.ChangeRegistration', params:{ " +
                    "language: " + language + ", " +
                    "appID: " + appID +
                    "}}")
        dataContainer.changeRegistrationTTSVR(language, appID);
        console.debug("exit");
    }

    function setGlobalProperties(helpPrompt, timeoutPrompt, appID) {
        var helpPromptLog = "",
            timeoutPromptLog = "";
        if (helpPrompt) {
            for (var i = 0; i < helpPrompt.length; i++) {
                helpPromptLog += "{type: " + helpPrompt[i].type + ", " +
                        "text: '" + helpPrompt[i].text + "'}, ";
            }
        }
        if (timeoutPrompt) {
            for (var i = 0; i < timeoutPrompt.length; i++) {
                timeoutPromptLog += "{type: " + timeoutPrompt[i].type + ", " +
                        "text: '" + timeoutPrompt[i].text + "'}, ";
            }
        }
        console.log("Message Received - {method: 'TTS.SetGlobalProperties', params:{ " +
                    "appID:" + appID + ", " +
                    "helpPrompt: [" + helpPromptLog + "], " +
                    "timeoutPrompt: [" + timeoutPromptLog + "]" +
                    "}}")

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

