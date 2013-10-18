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
import "../models"
import com.ford.sdl.hmi.log4cxx 1.0

GeneralView {
    applicationContext: true

    Rectangle {
        id: turnByTurnView
        anchors.fill: parent
        color: Constants.secondaryColor

        Item {
            // top 3/4 of screen
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: parent.height * 3/4

            Row {
                // top part for buttons
                id: top
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width
                height: 1/4 * parent.height
                spacing: (width - 4 * turnListButton.width) / 3

                OvalButton {
                    id: turnListButton
                    width: Constants.longOvalButtonWidth
                    anchors.verticalCenter: parent.verticalCenter
                    text: "TurnList"
                    fontSize: Constants.fontSize
                    onClicked: {
                        console.log("Go to TurnListView");
                        contentLoader.go("./views/TbtTurnListView.qml");
                    }
                }

                ListView {
                    id: softButtons
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width - turnListButton.width
                    height: parent.height
                    spacing: parent.spacing

                    orientation: ListView.Horizontal
                    interactive: false
                    model: dataContainer.navigationModel.softButtons

                    delegate: OvalButton {
                        anchors.verticalCenter: parent.verticalCenter
                        width: turnListButton.width
                        height: turnListButton.height
                        text: name
                        icon: (type !== Common.SoftButtonType.SBT_TEXT) ? image : undefined
                        fontSize: Constants.fontSize
                        highlighted: isHighlighted
                        onPressed: {
                            console.log("TBT view soft button: On pressed");
                            sdlButtons.onButtonEvent(Common.ButtonName.CUSTOM_BUTTON,
                                                     Common.ButtonEventMode.BUTTONDOWN,
                                                     buttonId);
                        }
                        onReleased: {
                            console.log("TBT view soft button: On released");
                            sdlButtons.onButtonEvent(Common.ButtonName.CUSTOM_BUTTON,
                                                     Common.ButtonEventMode.BUTTONUP,
                                                     buttonId);
                        }
                        onClicked: {
                            console.log("TBT view soft button: On clicked");
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
                }
            }

            Row {
                // mid part for picture, information.
                id: mid
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                width: parent.width
                height: 2/4 * parent.height
                spacing: 1/2 * turnIcon.width

                Icon {
                    id: turnIcon
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.height
                    height: parent.height
                    source: dataContainer.navigationModel.icon
                }

                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height
                    width: parent.width - turnIcon.width - spacing
                    spacing: (height - text1.height - text2.height - text3.height) / 2

                    Text {
                        id: text1
                        text: dataContainer.navigationModel.text1
                        color: Constants.primaryColor
                        font.pixelSize: Constants.fontSize
                    }

                    Text {
                        // TODO(KKolodiy): make 2 columns for this text, and change font
                        id: text2
                        text: "Distance to Maneur " + dataContainer.navigationModel.distanceToManeuver + "Total datance: " + dataContainer.navigationModel.totalDistance
                        color: Constants.primaryColor
                        font.pixelSize: Constants.fontSize

                    }

                    Text {
                        id: text3
                        text: dataContainer.navigationModel.text2
                        color: Constants.primaryColor
                        font.pixelSize: Constants.fontSize
                    }
                }
            }

            Item {
                id: bot
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: parent.width
                height: 1/4 * parent.height
            }

        }

        Item {
            // 1/4 bottom screen
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width
            height: 1/4 * parent.height
            BackButton { anchors.centerIn: parent }
        }
    }
}
