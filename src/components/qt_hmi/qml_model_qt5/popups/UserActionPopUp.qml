/**
 * @file PopUp.qml
 * @brief General popup view.
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
import "../controls"

Item {
    default property alias content: content.children
    property var callbackFunc: null
    property int padding: Constants.popUpPadding
    property string popUpName

    visible: false

    width: Constants.popupWidth
    height: Constants.popupHeigth

    MouseArea { anchors.fill: parent }

    Rectangle {
        width: parent.width / 2
        height: parent.height / 2
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        color: Constants.secondaryColor
        border.width: 1
        border.color: Constants.popUpBorderColor
        radius: padding
        Rectangle {
            id: content
            width: parent.width - padding
            height: parent.height - padding
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: Constants.transparentColor
        }

        Text {
            id: title
            text: "adsadad"
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            width: parent.width - padding
            height: 20
            font.pixelSize: Constants.fontSize
            color: Constants.primaryColor
            wrapMode: TextEdit.Wrap
        }

        Text {
            id: message
            text: "adsadad"
            anchors.top: parent.top
            anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            width: parent.width - padding
            height: 100
            font.pixelSize: 0
            color: Constants.primaryColor
            wrapMode: TextEdit.Wrap
        }

        OvalButton {
            id: okButton
            text: "Ok"
            fontSize: Constants.fontSize
            property bool result: true
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 15
            width: parent.width / 2 - 20
            onClicked: {
                deactivate(result)
            }
            visible: false
        }

        OvalButton {
            id: cancelButton
            text: "Cancel"
            fontSize: Constants.fontSize
            property bool result: false
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 15
            width: parent.width / 2 - 20
            onClicked: {
                deactivate(result)
            }
            visible: false
        }

        OvalButton {
            id: closeButton
            text: "Close"
            fontSize: Constants.fontSize
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 15
            width: parent.width - 20
            onClicked: {
                deactivate()
            }
            visible: false
        }

    }

    function activate(titleText, textBoxText, callback, buttons) {
        console.log("userActionPopUp activate enter");
        title.text = titleText;
        message.text = textBoxText;

        if (buttons) {
            callbackFunc = callback;
            okButton.visible = true
            cancelButton.visible = true
        } else {
            closeButton.visible = true
        }

        visible = true;
        console.debug("userActionPopUp activate exit");
    }

    function deactivate(result) {
        console.log("userActionPopUp deactivate enter");
        visible = false;
        title.text = "";
        message.text = "";
        if (callbackFunc) {
            callbackFunc(result);
            callbackFunc = null;
        }
        okButton.visible = false
        cancelButton.visible = false
        closeButton.visible = false
        console.debug("userActionPopUp deactivate exit");
    }
}
