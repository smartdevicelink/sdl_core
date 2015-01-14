/**
 * @file SDLNonMediaView.qml
 * @brief SDL non media screen view.
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
import "../models"
import "../controls"
import "../models/Constants.js" as Constants
import "../models/Internal.js" as Internal
import "../hmi_api/Common.js" as Common

GeneralView {
    applicationContext: true

    Item {
        id: upperContent
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left

        Item {
            id: topButtonRow
            // top 1/4 of screen
            width: parent.width
            height: 1/4 * parent.height

            PagedFlickable {
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width
                elementWidth: Constants.ovalButtonWidth
                spacing: (width - 4 * elementWidth) / 3
                snapTo: elementWidth + spacing

                OvalButton {
                    text: "Options"
                    onClicked: { contentLoader.go("./views/SDLPlayerOptionsListView.qml") }
                }

                Repeater {
                    model: dataContainer.currentApplication.softButtons ?
                               dataContainer.currentApplication.softButtons.count :
                               0
                    delegate:
                        SoftButton {
                            appId: dataContainer.currentApplication.appId
                            button: dataContainer.currentApplication.softButtons.get(index)
                        }
                }
            }
        }

        Column {
            id: midContent
            height: parent.height * 3/4
            width: parent.width
            anchors.top: topButtonRow.bottom
            anchors.left: parent.left

            Text {
                id: deviceName
                width: parent.width
                height: 1/5 * parent.height
                text: "Device: " + dataContainer.currentApplication.deviceName
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: Constants.fontSize
                color: Constants.primaryColor
            }

            Row {
                width: parent.width
                height: 4/5 * parent.height
                spacing: Constants.margin

                Image {
                    id: image
                    height: parent.height
                    width: height
                    source: dataContainer.currentApplication.hmiUIText.image
                }

                Column {
                    height: parent.height
                    width: parent.width - image.width - parent.spacing
                    spacing: Constants.margin

                    Text {
                        id: text1
                        width: parent.width
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: dataContainer.currentApplication.hmiUITextAlignment
                        text: dataContainer.currentApplication.hmiUIText.mainField1
                        font.pixelSize: Constants.fontSize
                        color: Constants.primaryColor
                    }

                    Text {
                        width: parent.width
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: dataContainer.currentApplication.hmiUITextAlignment
                        text: dataContainer.currentApplication.hmiUIText.mainField2
                        font.pixelSize: Constants.fontSize
                        color: Constants.primaryColor
                    }
                }
            }
        }
    }

    Item {
        // bottom 1/4 screen
        id: lowerContent
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: parent.height * 1/4

        PresetRow {
            id: presetsRow
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            presets: Internal.getArrayForPresetRow(dataContainer.currentApplication)
            width: parent.width
            property bool clickProcessed

            onPresetButtonPressed: {
                sdlButtons.onButtonEvent(Common.ButtonName.PRESET_0 + selectedIndex, Common.ButtonEventMode.BUTTONDOWN, undefined)
            }

            onPresetButtonReleased: {
                sdlButtons.onButtonEvent(Common.ButtonName.PRESET_0 + selectedIndex, Common.ButtonEventMode.BUTTONUP, undefined)
            }

	    onPresetButtonClicked: {
	        sdlButtons.onButtonPress(Common.ButtonName.PRESET_0 + selectedIndex, Common.ButtonPressMode.SHORT, undefined)
	    }

	    onPresetButtonHold: {
	        sdlButtons.onButtonPress(Common.ButtonName.PRESET_0 + selectedIndex, Common.ButtonPressMode.LONG, undefined)
	    }
        }
    }

    StatusBar {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }
}
