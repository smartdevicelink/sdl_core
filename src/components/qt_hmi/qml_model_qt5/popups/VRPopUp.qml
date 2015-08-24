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
        anchors.left: parent.left
        anchors.top: parent.top
        height: voice.height
        width: parent.width - voice.width
        text: "Speak the command"
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: Constants.fontSize
        color: Constants.primaryColor
    }

    Image {
        id: voice
        anchors.top: parent.top
        anchors.right: parent.right
        source: "../res/controlButtons/vrImage.png"
    }
    OvalButton{
        id:helpButton
        anchors.rightMargin: Constants.popupMargin
        anchors.leftMargin: Constants.popupMargin
        anchors.top: voice.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        text: "Help"
        onClicked:{
            if (interactionPopup.performInteractionIsActiveNow)
                ttsPopUp.activate(ttsPopUp.helpPromptstr)
            if (dataContainer.activeVR) {
                vrPopUp.complete();
                vrHelpPopup.hide()
            }
        }
    }

    ScrollableListView {
        anchors.bottomMargin: Constants.popupMargin
        anchors.rightMargin: Constants.popupMargin
        anchors.leftMargin: Constants.popupMargin
        anchors.top: helpButton.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        model: if (interactionPopup.grammarID) {
                   dataContainer.choicesVrCommands
               }
               else {
                   dataContainer.vrCommands
               }

        delegate: OvalButton {
            width: parent.width
            text: command
            visible: visibleButtons(grammarID,type)
            onClicked: {
                if (interactionPopup.performInteractionIsActiveNow && type === Common.VRCommandType.Choice)
                    interactionPopup.complete(Common.Result.SUCCESS, {"choiceID": cmdID})
                sdlVR.onCommand(cmdID, appID === 0 ? undefined : appID);
                if (dataContainer.activeVR) {
                    vrPopUp.complete();
                    vrHelpPopup.hide()
                }
            }
        }
    }

    function activate() {
        dataContainer.activeVR = true;
        sdlVR.started();
        show();
    }

    function complete(reason) {
        dataContainer.activeVR = false;
        sdlVR.stopped();
        hide();
    }

    function sortModelforPerformInteraction() {
        var n,
            i,
            j;
        for (n = 0; n < dataContainer.choicesVrCommands.count; n++) {
            for (i = n + 1; i < dataContainer.choicesVrCommands.count; i++) {
                if (dataContainer.choicesVrCommands.get(n).type === Common.VRCommandType.Command &&
                        dataContainer.choicesVrCommands.get(i).type === Common.VRCommandType.Choice) {
                    dataContainer.choicesVrCommands.move(i, n, 1);
                    n = 0;
                }
            }
        }
        for (j = interactionPopup.grammarID.length; j > 0; j--) {
            for (n = 0; n < dataContainer.choicesVrCommands.count &&
                 dataContainer.choicesVrCommands.get(n).type === Common.VRCommandType.Choice; n++) {
                for (i = n + 1; i < dataContainer.choicesVrCommands.count &&
                     dataContainer.choicesVrCommands.get(i).type === Common.VRCommandType.Choice; i++) {
                    if (dataContainer.choicesVrCommands.get(n).grammarID !== interactionPopup.grammarID[j-1]
                            && dataContainer.choicesVrCommands.get(i).grammarID === interactionPopup.grammarID[j-1]) {
                        dataContainer.choicesVrCommands.move(i, n, 1);
                        n = 0;
                    }
                }
            }
        }
    }

    function visibleButtons(grammarID, type) {
        if (interactionPopup.grammarID) {
            return interactionPopup.grammarID.indexOf(grammarID) !== -1
            }
        else {
           return type === Common.VRCommandType.Command
            }
        }
}
