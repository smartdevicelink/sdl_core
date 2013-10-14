/**
 * @file TurnByTurnView.qml
 * @brief View for TurnByTurn.
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
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants

GeneralView {
    width: 647
    height: 389
    applicationContext: true

    Rectangle {
        property string textColor: Constants.primaryColor
        property string backgroundColor: "#000000"
        //TODO {ALeshin}: Remove this color while refactoring

        id: main
        color: backgroundColor
        anchors.fill: parent

        Rectangle {
            id: head
            height: 50
            border.width: 2
            border.color: parent.textColor
            color: parent.backgroundColor
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5

            Text {
                text: dataContainer.navigationModel.text1
                verticalAlignment: Text.AlignVCenter
                anchors.topMargin: 5
                anchors.bottomMargin: 5
                anchors.rightMargin: 5
                anchors.leftMargin: 10
                anchors.fill: parent
                font.pixelSize: 14
                color: parent.parent.textColor
            }
        }

        Rectangle {
            id: turnIcon
            width: 210
            height: 210
            border.color: parent.textColor
            color: parent.backgroundColor
            anchors.top: head.bottom
            anchors.topMargin: -2
            anchors.right: parent.right
            anchors.rightMargin: 5

            Icon {
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                anchors.bottomMargin: 5
                anchors.topMargin: 5
                anchors.fill: parent
                source: dataContainer.navigationModel.icon
            }
        }

        Rectangle {
            id: other
            width: 210
            height: 40
            color: parent.backgroundColor
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.topMargin: 5
            anchors.top: turnIcon.bottom
            Text {
                color: parent.parent.textColor
                text: dataContainer.navigationModel.text2
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
                anchors.rightMargin: 5
                anchors.topMargin: 5
                anchors.leftMargin: 10
                font.pixelSize: 14
                anchors.bottomMargin: 5
            }
            border.color: parent.textColor
        }

        OvalButton {
            id: turnList
            height: 65
            fontSize: 18
            text: "Turn List"
            anchors.top: row.bottom
            anchors.topMargin: 0
            anchors.right: turnIcon.left
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
        }

        ListView {
            id: softButton
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.right: turnIcon.left
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: turnList.bottom
            anchors.topMargin: 5
            delegate: OvalButton {
                width: parent.width
                height: 65
                text: name
                icon: image
                highlighted: isHighlighted
                onPressed: {
                    console.log("On pressed");
                    sdlButtons.onButtonEvent(Common.ButtonName.CUSTOM_BUTTON,
                                             Common.ButtonEventMode.BUTTONDOWN,
                                             buttonId);
                }
                onReleased: {
                    console.log("On released");
                    sdlButtons.onButtonEvent(Common.ButtonName.CUSTOM_BUTTON,
                                             Common.ButtonEventMode.BUTTONUP,
                                             buttonId);
                }
                onClicked: {
                    console.log("On clicked");
                    sdlButtons.onButtonPress(Common.ButtonName.CUSTOM_BUTTON,
                                             Common.ButtonPressMode.SHORT,
                                             buttonId);
                }
                onPressAndHold: {
                    console.log("On press and hold");
                    sdlButtons.onButtonPress(Common.ButtonName.CUSTOM_BUTTON,
                                             Common.ButtonPressMode.LONG,
                                             buttonId);
                }
                // TODO(KKolodiy): System action doesn't work in WebHMI
            }
            model: dataContainer.navigationModel.softButtons
        }

        OvalButton {
            id: home
            text: "Home Screen"
            anchors.top: other.bottom
            anchors.topMargin: 5
            anchors.left: softButton.right
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 5
            icon: dataContainer.getApplication(dataContainer.navigationModel.appId).icon
            onClicked: contentLoader.back()
        }

        Row {
            id: row
            height: 50
            spacing: 10
            anchors.top: head.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.right: turnIcon.left
            anchors.rightMargin: 5

            Rectangle {
                id: totalDistance
                width: parent.width/2 - 5
                color: "#00000000"
                border.color: parent.parent.textColor
                anchors.bottom: parent.bottom
                anchors.top: parent.top

                Text {
                    text: dataContainer.navigationModel.totalDistance
                    verticalAlignment: Text.AlignVCenter
                    anchors.topMargin: 5
                    anchors.bottomMargin: 5
                    anchors.rightMargin: 5
                    anchors.leftMargin: 10
                    anchors.fill: parent
                    font.pixelSize: 14
                    color: parent.parent.parent.textColor
                }
            }

            Rectangle {
                id: eta
                width: parent.width/2 - 5
                color: "#00000000"
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                border.color: parent.parent.textColor

                Text {
                    color: parent.parent.parent.textColor
                    text: dataContainer.navigationModel.eta
                    verticalAlignment: Text.AlignVCenter
                    anchors.fill: parent
                    anchors.rightMargin: 5
                    anchors.topMargin: 5
                    anchors.leftMargin: 10
                    font.pixelSize: 14
                    anchors.bottomMargin: 5
                }
            }
        }
    }
}
