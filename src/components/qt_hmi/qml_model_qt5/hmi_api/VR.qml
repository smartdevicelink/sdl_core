/**
 * @file VR.qml
 * @brief Implement of interface VR.
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
Item {
    function isReady() {
        console.log("Message Received - {method: 'VR.IsReady'}")
        return {
            available: dataContainer.hmiVRAvailable
        }
    }

    function addCommand(cmdID, vrCommands, type, grammarID, appID) {
        var vrCommandsLog = "";
        if (vrCommands) {
            for (var i = 0; i < vrCommands.length; i++) {
                vrCommandsLog += "'" + vrCommands[i] + "', ";
            }
        }
        console.log("Message Received - {method: 'VR.AddCommand', params:{ " +
                    "vrCommands: [" + vrCommandsLog + "], " +
                    "cmdID: " + cmdID + ", " +
                    "appID: " + appID + ", " +
                    "type: " + type + ", " +
                    "grammarID: " + grammarID +
                    "}}")
        for (var i = 0; i < vrCommands.length; ++i) {
            if (type === Common.VRCommandType.Command) {
                dataContainer.vrCommands.append({
                                               cmdID: cmdID,
                                               command: vrCommands[i],
                                               appID: appID === undefined ? 0 : appID,
                                               type: type,
                                               grammarID: grammarID,
                                           });
            }
            else {
                dataContainer.choicesVrCommands.append({
                                                   cmdID: cmdID,
                                                   command: vrCommands[i],
                                                   appID: appID === undefined ? 0 : appID,
                                                   type: type,
                                                   grammarID: grammarID,
                                               });
            }
        }
        console.log("exit")
    }

    function deleteCommand(cmdID, appID) {
        console.log("Message Received - {method: 'VR.DeleteCommand', params:{ " +
                    "appID: " + appID + ", " +
                    "cmdID: " + cmdID +
                    "}}")
        for (var i = 0; i < dataContainer.vrCommands.count; ) {
            if ((dataContainer.vrCommands.get(i).cmdID === cmdID) &&
                    ((appID === undefined) || (dataContainer.vrCommands.get(i).appID === appID))) {
                dataContainer.vrCommands.remove(i);
                continue;
            }
            ++i;
        }
        console.log("exit")
    }

    function getLanguage() {
        console.log("Message Received - {method: 'VR.GetLanguage'}")
        return {
            language: dataContainer.hmiTTSVRLanguage
        }
    }

    function getSupportedLanguages() {
        console.log("Message Received - {method: 'VR.GetSupportedLanguages'}")
        return {
            languages: settingsContainer.sdlLanguagesList
        }
    }

    function getCapabilities() {
        console.log("Message Received - {method: 'UI.GetCapabilities'}")
        return {
            vrCapabilities: [ Common.VrCapabilities.VR_TEXT ]
        }
    }

    function changeRegistration(language, appID) {
        console.debug("enter");
        console.log("Message Received - {method: 'VR.ChangeRegistration', params:{ " +
                    "language: " + language + ", " +
                    "appID: " + appID +
                    "}}")
        dataContainer.changeRegistrationTTSVR(language, appID);
        console.debug("exit");
    }
    function ttsChunksToString(ttsChunks){
        return ttsChunks.map(function(str) { return str.text }).join('\n')
    }
    function performInteraction(helpPrompt, initialPrompt, timeoutPrompt, timeout, grammarID) {
        console.debug("enter");
        var helpttsChunksLog = "",
            initialttsChunkLog = "",
            timeoutttsChunkLog = "",
            grammarIDLog ="";

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
        interactionPopup.grammarID = grammarID
        console.debug("exit");
    }
}
