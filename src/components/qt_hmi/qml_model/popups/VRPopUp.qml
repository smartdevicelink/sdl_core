/**
 * @file VRPopUp.qml
 * @brief Popup view for VR interface (list commands).
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
import "../models/Constants.js" as Constants
import "../hmi_api/Common.js" as Common
import "../views"
import "../controls"

PopUp {
    Text {
        id: title
        height: 50
        text: "Speak the command"
        verticalAlignment: Text.AlignVCenter
        anchors.right: voice.left
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 15
        font.pixelSize: 14
        color: Constants.primaryColor
    }

    Image {
        id: voice
        x: 591
        width: 50
        height: 50
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 15
        source: "../res/controlButtons/vrImage.png"
    }

    ListView {
        id: listCommands
        anchors.bottomMargin: 15
        anchors.rightMargin: 15
        anchors.leftMargin: 15
        anchors.top: voice.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0
        clip: true
        model: dataContainer.vrCommands
        delegate: OvalButton {
            width: parent.width
            text: command
            onClicked: {
                sdlVR.onCommand(cmdID, appID);
            }
        }
        Rectangle {
            id: scrollbar
            visible: true
            anchors.right: listCommands.right
            y: listCommands.visibleArea.yPosition * listCommands.height
            width: 5
            height: listCommands.visibleArea.heightRatio * listCommands.height
            color: Constants.primaryColor
        }
    }

    function activate() {
        dataContainer.activeVR = true;
        dataContainer.systemSavedContext = dataContainer.systemContext
        dataContainer.systemContext = Common.SystemContext.SYSCTXT_VRSESSION
        sdlVR.started();
        show();
    }

    function deactivate() {
        dataContainer.activeVR = false;
        dataContainer.systemContext = dataContainer.systemSavedContext
        sdlVR.stopped();
        hide();
    }
}
