/**
 * @file NavigationKeyboardView.qml
 * @brief Navigation keyboard screen view.
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

GeneralView {
    Item {
        id: navKeyboard
        anchors.fill: parent

        Item {
            // 3/4 top screen
            height: parent.height * 3/4
            width: parent.width
            anchors.top: parent.top
            anchors.left: parent.left

            // Text line with input text
            TextInput {
                id: inputText
                anchors.top:parent.top
                anchors.left:parent.left
                maximumLength: 30
                color: Constants.primaryColor
                font.pixelSize: Constants.fontSize
                text: dataContainer.routeText
            }

            // Keyboard
            Column {
                anchors.centerIn: parent
                Row {
                    id: upperRow
                    property string qWERTY : "QWERTYUIOP"
                    anchors.horizontalCenter: parent.horizontalCenter

                    Repeater {
                        model: 10
                        CircleButton {
                            imgOff: "../res/buttons/preset_btn.png"
                            imgOn: "../res/buttons/preset_pressed_btn.png"
                            text: upperRow.qWERTY.charAt(index)
                            pixelSize: Constants.fontSize

                            onClicked: {
                                dataContainer.routeText += upperRow.qWERTY.charAt(index)
                            }
                        }
                    }
                }
                Row {
                    id: middleRow
                    property string aSDFGH : "ASDFGHJKL"
                    anchors.horizontalCenter: parent.horizontalCenter

                    CircleButton {
                        imgOff: "../res/buttons/preset_btn.png"
                        imgOn: "../res/buttons/preset_pressed_btn.png"
                        text: "123"
                        pixelSize: 15
                    }
                    //--------------
                    Repeater {
                        model: 9
                        CircleButton {
                            imgOff: "../res/buttons/preset_btn.png"
                            imgOn: "../res/buttons/preset_pressed_btn.png"
                            text: middleRow.aSDFGH.charAt(index)
                            pixelSize: Constants.fontSize

                            onClicked: {
                                dataContainer.routeText += middleRow.aSDFGH.charAt(index)
                            }
                        }
                    }
                    //--------------
                    CircleButton {
                        imgOff: "../res/buttons/preset_btn.png"
                        imgOn: "../res/buttons/preset_pressed_btn.png"
                        onIsPressedChanged: {
                            hoverImg.source = isPressed ? "../res/nav/delete_icon_black.png" : "../res/nav/delete_icon.png";
                        }
                        Image {
                            id: hoverImg
                            anchors.centerIn: parent
                            source: "../res/nav/delete_icon.png"
                        }

                        onClicked: {
                            dataContainer.routeText = dataContainer.routeText.substring(0,dataContainer.routeText.length - 1)
                        }
                    }
                }
                Row {
                    id: lowerRow
                    property string zXCVBN : "ZXCVBNM"
                    anchors.horizontalCenter: parent.horizontalCenter

                    CircleButton {
                        imgOff: "../res/buttons/preset_btn.png"
                        imgOn: "../res/buttons/preset_pressed_btn.png"
                        text: "!@#"
                        pixelSize: 15
                    }
                    //-----------------
                    Repeater {
                        model: 7
                        CircleButton {
                            imgOff: "../res/buttons/preset_btn.png"
                            imgOn: "../res/buttons/preset_pressed_btn.png"
                            text: lowerRow.zXCVBN.charAt(index)
                            pixelSize: Constants.fontSize

                            onClicked: {
                                dataContainer.routeText += lowerRow.zXCVBN.charAt(index)
                            }
                        }
                    }
                    //----------------
                    CircleButton {
                        imgOff: "../res/buttons/preset_btn.png"
                        imgOn: "../res/buttons/preset_pressed_btn.png"
                        text: "Clear"
                        pixelSize: 12

                        onClicked: {
                            dataContainer.routeText = ""
                        }
                    }
                    //----------------
                    CircleButton {
                        imgOff: "../res/buttons/preset_btn.png"
                        imgOn: "../res/buttons/preset_pressed_btn.png"
                        text: "Space"
                        pixelSize: 12

                        onClicked: {
                            dataContainer.routeText += " "
                        }
                    }
                }

            } // column

           Image {
                id: goBtn
                anchors.right: parent.right
                anchors.top: parent.top
                source: "../res/buttons/short_oval_btn.png"

                Text {
                    id: goText
                    anchors.centerIn: parent
                    text: "Go"
                    color: Constants.primaryColor
                    font.pixelSize: Constants.fontSize
                }

                MouseArea {
                    anchors.fill: parent

                    onPressed: {
                        goBtn.source = "../res/buttons/short_oval_btn_pressed.png"
                        goText.color = Constants.pressedButtonTextColor
                    }

                    onReleased: {
                        goBtn.source = "../res/buttons/short_oval_btn.png"
                        goText.color = Constants.primaryColor
                    }

                    onClicked: {
                        contentLoader.go("./views/NavigationInRouteGridView.qml")
                    }
                }
            }
        } // 3/4 top screen

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
