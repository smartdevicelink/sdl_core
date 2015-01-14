/**
 * @file PerformAudioPassThruPopup.qml
 * @brief Popup for audio pass thru
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
import "../models/Constants.js" as Constants
import "../hmi_api/Common.js" as Common
import "../controls"

ContextPopup {
    property variant async

    function showAudioPassThru(){
        console.debug("entered")
        dataContainer.uiAudioPassThru.running = true
        show()
        if (dataContainer.uiAudioPassThru.timeout) {
            timer.start()
        }
        console.debug("exited")
    }

    function complete(reason){
        console.debug("enter", reason)
        timer.stop()
        dataContainer.uiAudioPassThru.timeout = 0
        dataContainer.uiAudioPassThru.running = false
        switch (reason) {
        case Common.Result.ABORTED:
            console.debug("exit with abort")
            DBus.sendError(async, Common.Result.ABORTED)
            break;
        case Common.Result.SUCCESS:
            console.debug("exit with success")
            DBus.sendReply(async, {})
            break;
        case Common.Result.RETRY:
            console.debug("exit with retry")
            DBus.sendError(async, Common.Result.RETRY)
            break;
        }
        hide()
    }


    Column {
        spacing: Constants.generalSpacing
        anchors.centerIn: parent
        Timer {
            id: timer
            interval: dataContainer.uiAudioPassThru.timeout
            onTriggered: {
                complete(Common.Result.SUCCESS)
            }
        }

        Text {
            id: appNameText
            anchors.horizontalCenter: parent.horizontalCenter
            text: dataContainer.uiAudioPassThru.appName
            color: Constants.primaryColor
            font.pixelSize: Constants.fontSize * 2
        }

        Row {
            spacing: Constants.generalSpacing
            Image {
                source: "../res/controlButtons/vrImage.png"
            }

            Column {
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: dataContainer.uiAudioPassThru.firstLine
                    color: Constants.audioPassThruTextColor
                    font.pixelSize: Constants.fontSize
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: dataContainer.uiAudioPassThru.secondLine
                    color: Constants.audioPassThruTextColor
                    font.pixelSize: Constants.fontSize
                }
            }
        }


        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: Constants.generalSpacing

            OvalButton {
                text: "Retry"
                fontSize: Constants.fontSize
                onClicked: {
                    complete(Common.Result.RETRY)
                }
            }

            OvalButton {
                text: "Done"
                fontSize: Constants.fontSize
                onClicked: {
                    complete(Common.Result.SUCCESS)
                }
            }
        }

        OvalButton {
            text: "Close"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                console.debug("enter")
                complete(Common.Result.ABORTED)
                console.debug("exit")
            }
        }
    }
}
