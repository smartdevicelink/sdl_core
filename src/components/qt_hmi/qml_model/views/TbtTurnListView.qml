/**
 * @file TbtTurnListView.qml
 * @brief View for TurnByTurn list.
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
import "../controls"
import "../models/Constants.js" as Constants
import "../hmi_api/Common.js" as Common

GeneralView {
    width: 600
    height: 300
    applicationContext: true

    Text {
        id: title
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 5
        font.pixelSize: 14
        color: Constants.primaryColor
        text: "Turn List"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
    }

    ListView {
        id: turnList
        anchors.topMargin: 0
        anchors.bottom: softButton.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: title.bottom
        anchors.bottomMargin: 0
        delegate: ListItem {
            height: Constants.itemListHeight
            width: parent.width
            text: name.fieldText
            icon: image
        }
        model: dataContainer.getApplication(
                   dataContainer.navigationModel.appId).turnList
    }

    ListView {
        id: softButton
        height: 65
        anchors.bottom: back.top
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        delegate: OvalButton {
            width: parent.width
            text: name
            icon: (type !== Common.SoftButtonType.SBT_TEXT) ? image : undefined
            highlighted: isHighlighted
            onPressed: {
                console.log("TbtTurnList view soft button: On press and hold");
                sdlButtons.onButtonEvent(Common.ButtonName.CUSTOM_BUTTON,
                                         Common.ButtonEventMode.BUTTONDOWN,
                                         buttonId);
            }
            onReleased: {
                console.log("TBT view soft button: On press and hold");
                sdlButtons.onButtonEvent(Common.ButtonName.CUSTOM_BUTTON,
                                         Common.ButtonEventMode.BUTTONUP,
                                         buttonId);
            }
            onClicked: {
                console.log("TBT view soft button: On press and hold");
                sdlButtons.onButtonPress(Common.ButtonName.CUSTOM_BUTTON,
                                         Common.ButtonPressMode.SHORT,
                                         buttonId);
            }
            onPressAndHold: {
                console.log("TBT view soft button: On press and hold");
                sdlButtons.onButtonPress(Common.ButtonName.CUSTOM_BUTTON,
                                         Common.ButtonPressMode.LONG,
                                         buttonId);
            }
            // TODO(KKolodiy): System action doesn't work in WebHMI
        }
        model: dataContainer.getApplication(
                   dataContainer.navigationModel.appId).turnListSoftButtons
    }

    OvalButton {
        id: back
        width: Constants.longOvalButtonWidth
        text: "Back"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: contentLoader.back()
    }
}
