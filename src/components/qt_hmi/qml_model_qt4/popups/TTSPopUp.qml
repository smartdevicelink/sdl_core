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

import QtQuick 1.1
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants
import "../controls"

PopUp {
    height: Constants.ttsPopUpHeight
    width: Constants.ttsPopUpWidth
    padding: Constants.ttsPopUpPadding
    property variant async

    ScrollableText {
        id: text
        anchors.fill: parent
    }

    resources: [
        Timer {
            id: timer
            interval: Constants.ttsSpeakTime
                    onTriggered: deactivate()
        }
    ]

    function activate(message) {
        console.debug("Activate TTS popup:", message);
        dataContainer.activeTTS = true;
        text.text = message;
        show();
        timer.restart();
        console.debug("Exit");
    }

    function deactivate() {
        console.debug("Deactivate TTS popup");
        dataContainer.activeTTS = false;
        text.text = '';
        timer.stop();
        hide();
        DBus.sendReply(async, {});
        async = null;
        console.debug("Exit");
    }
}

