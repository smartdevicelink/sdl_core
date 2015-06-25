/**
 * @file TTSPopUp.qml
 * @brief Popup view for TTS
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
import QtQuick.Controls 1.0
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants

PopUp {
    height: Constants.ttsPopUpHeight
    width: Constants.ttsPopUpWidth
    padding: Constants.ttsPopUpPadding
    property var async
    property string helpPromptstr

    ScrollView {
        anchors.fill: parent
        Text {
            id: text
            anchors.fill: parent
            color: Constants.popUpBorderColor
            font.pixelSize: Constants.ttsFontSize
            text:""
        }
    }

    resources: [
        Timer {
            id: timer
            interval: Constants.ttsSpeakTime
                    onTriggered: deactivate()
        },

        Timer {
            id: ttsPerformInteractionTimer
            interval: Constants.ttsSpeakTime
            onTriggered:
                if(interactionPopup.performInteractionIsActiveNow)
                    activate(message)
            property var message: undefined
        }
    ]

    function performInteraction(helpPrompt, initialPrompt, timeoutPrompt, timeout) {
        console.debug("Activate TTS popup:", "message");
        helpPromptstr = helpPrompt
        activate(initialPrompt);
        if (timeout * 2 - Constants.ttsSpeakTime > 0) {
            ttsPerformInteractionTimer.message = timeoutPrompt;
            ttsPerformInteractionTimer.interval = timeout - Constants.ttsSpeakTime;
            ttsPerformInteractionTimer.restart()
        }
    }

    function activate(message) {
        console.debug("Activate TTS popup:", message);
        dataContainer.activeTTS = true;
        sdlTTS.started();
        console.debug("TTS started");
        text.text = message;
        show();
        timer.restart();
        console.debug("Exit");
    }

    function deactivate() {
        console.debug("Deactivate TTS popup");
        dataContainer.activeTTS = false;
        sdlTTS.stopped();
        console.debug("TTS stopped");
        text.text = '';
        timer.stop();
        hide();
        DBus.sendReply(async, {});
        async = null;
        console.debug("Exit");
    }
}

