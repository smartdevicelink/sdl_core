/**
 * @file PhoneKeyboardView.qml
 * @brief Phone keyboard screen view.
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
import "../controls"
import "../models"
import "../models/Constants.js" as Constants

GeneralView{
    Component.onCompleted: phoneKeyboard.changeColorOfActiveButtons()

    Item {
        function changeColorOfActiveButtons() {
            for (var i = 0; i < contactsListModel.count; i++) {
                activeButtons[contactsListModel.get(i).name[0].toUpperCase()] = true
            }
        }

        id: phoneKeyboard
        anchors.fill: parent

        property variant activeButtons: {
            "A": false, "B": false, "C": false, "D": false, "E": false,
            "F": false, "G": false, "H": false, "I": false, "J": false,
            "K": false, "L": false, "M": false, "N": false, "O": false,
            "P": false, "Q": false, "R": false, "S": false, "T": false,
            "U": false, "V": false, "W": false, "X": false, "Y": false, "Z": false
        }

        ContactsListModel {
            id: contactsListModel
        }

        Item {
            // 3/4 top screen
            height: parent.height * 3/4
            width: parent.width
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            Column {
                anchors.centerIn: parent

                Row {
                    id: upperRow
                    property string contentLoader : "ABCDEFGHI"
                    anchors.horizontalCenter: parent.horizontalCenter
                    Repeater {
                        model: 9
                        CircleButton {
                            imgOff: "../res/buttons/preset_btn.png"
                            imgOn: "../res/buttons/preset_pressed_btn.png"
                            textColorDefault: phoneKeyboard.activeButtons[upperRow.contentLoader.charAt(index)]
                                              ? Constants.primaryColor : Constants.inactiveButtonTextColor
                            text: upperRow.contentLoader.charAt(index)
                            pixelSize: Constants.fontSize
                            onIsPressedChanged: {
                                if (!isPressed){
                                    if (phoneKeyboard.activeButtons[upperRow.contentLoader.charAt(index)]) {
                                        dataContainer.contactsFirstLetter = text
                                        contentLoader.go("./views/ContactsListView.qml")
                                    }
                                }
                            }
                        }
                    }
                }

                Row {
                    id: middleRow
                    property string contentLoader : "JKLMNOPQR"
                    anchors.horizontalCenter: parent.horizontalCenter
                    Repeater {
                        model: 9
                        CircleButton {
                            imgOff: "../res/buttons/preset_btn.png"
                            imgOn: "../res/buttons/preset_pressed_btn.png"
                            textColorDefault: phoneKeyboard.activeButtons[middleRow.contentLoader.charAt(index)]
                                              ? Constants.primaryColor : Constants.inactiveButtonTextColor
                            text: middleRow.contentLoader.charAt(index)
                            pixelSize: Constants.fontSize
                            onIsPressedChanged: {
                                if (!isPressed){
                                    if (phoneKeyboard.activeButtons[middleRow.contentLoader.charAt(index)]) {
                                        dataContainer.contactsFirstLetter = text
                                        contentLoader.go("./views/ContactsListView.qml")
                                    }
                                }
                            }
                        }
                    }
                }

                Row {
                    id: lowerRow
                    property string contentLoader : "STUVWXYZ"
                    anchors.horizontalCenter: parent.horizontalCenter
                    Repeater {
                        model: 8
                        CircleButton {
                            imgOff: "../res/buttons/preset_btn.png"
                            imgOn: "../res/buttons/preset_pressed_btn.png"
                            textColorDefault: phoneKeyboard.activeButtons[lowerRow.contentLoader.charAt(index)]
                                              ? Constants.primaryColor : Constants.inactiveButtonTextColor
                            text: lowerRow.contentLoader.charAt(index)
                            pixelSize: Constants.fontSize
                            onIsPressedChanged: {
                                if (!isPressed){
                                    if (phoneKeyboard.activeButtons[lowerRow.contentLoader.charAt(index)]) {
                                        dataContainer.contactsFirstLetter = text
                                        contentLoader.go("./views/ContactsListView.qml")
                                    }
                                }
                            }
                        }
                    }
                    CircleButton {
                        imgOff: "../res/buttons/preset_btn.png"
                        imgOn: "../res/buttons/preset_pressed_btn.png"
                        text: "123"
                        pixelSize: 20
                    }
                }
            }
        }

        Item {
            // 1/4 bottom screen
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width
            height: 1/4 * parent.height

            BackButton { anchors.centerIn: parent}
        }
    }
}


