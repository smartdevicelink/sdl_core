/**
 * @file SDLNavi.qml
 * @brief SDL navigation screen view.
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
import QtMultimedia 5.0
import "../controls"
import "../models/Constants.js" as Constants
import "../hmi_api/Common.js" as Common

GeneralView {
    applicationContext: true

    MultiTouchArea {
        anchors.fill: parent
        id: wholeWindow

        Row {
            // Top items
            id: topItems
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 1/6 * parent.height
            spacing: Constants.sdlNaviSpacing

            Rectangle {
                anchors.top: parent.top
                width: 1/5 * parent.width
                height: 4/5 * parent.height
                color: Constants.sdlNaviTransparentItemColor
                radius: 5

                Text {
                    text: dataContainer.currentApplication.hmiUIText.mainField1
                    anchors.fill: parent
                    font.pixelSize: Constants.fontSize
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Rectangle {
                anchors.top: parent.top
                width: 3/5 * parent.width - 2 * parent.spacing
                height: parent.height
                color: Constants.sdlNaviTransparentItemColor
                radius: 5

                Text {
                    text: dataContainer.currentApplication.hmiUIText.mainField2
                    anchors.fill: parent
                    font.pixelSize: Constants.fontSize
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Rectangle {
                id: rightRectOfTopItems
                anchors.top: parent.top
                width: 1/5 * parent.width
                height: 4/5 * parent.height
                color: Constants.sdlNaviTransparentItemColor
                radius: 5
                Text {
                    text: dataContainer.currentApplication.hmiUIText.mainField3 ? dataContainer.currentApplication.hmiUIText.mainField3 : "field3"
                    anchors.fill: parent
                    font.pixelSize: Constants.fontSize
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        Column {
            // "+", "-" button at left
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: 1/12 * parent.width
            height: 2/10 * parent.height + spacing
            spacing: Constants.sdlNaviSpacing

            GradientRectangle {
                width: parent.width
                height: 1/10 * wholeWindow.height
                text: "+"
                isCustomButton: true
                customButtonID: 3
            }
            GradientRectangle {
                width: parent.width
                height: 1/10 * wholeWindow.height
                text: "-"
                isCustomButton: true
                customButtonID: 4
            }
        }

        Column {
            anchors.right: parent.right
            anchors.top: topItems.bottom
            width: 1/6 * parent.width
            height: 2/10 * parent.height + spacing
            spacing: Constants.sdlNaviSpacing

            Rectangle {
                width: parent.width
                height: 1/10 * wholeWindow.height
                color: Constants.sdlNaviTransparentItemColor
                radius: 5
                Text {
                    text: ""
                    anchors.fill: parent
                    font.pixelSize: Constants.fontSize
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Rectangle {
                width: parent.width
                height: 1/10 * wholeWindow.height
                color: Constants.sdlNaviTransparentItemColor
                radius: 5
                Text {
                    text: dataContainer.currentApplication.hmiUIText.mainField4 ? dataContainer.currentApplication.hmiUIText.mainField4 : "mainField4"
                    anchors.fill: parent
                    font.pixelSize: Constants.fontSize
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        Row {
            // bottom items
            id: bottomItems
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width
            height: 1/6 * parent.height
            spacing: Constants.sdlNaviSpacing

            GradientRectangle {
                anchors.bottom: parent.bottom
                width: 1/3 * parent.width
                height: parent.height
                text: "Menu"
                isCustomButton: true
                customButtonID: 1
            }
            Rectangle {
                anchors.bottom: parent.bottom
                width: 1/3 * parent.width - 2 * parent.spacing
                height: parent.height
                color: Constants.sdlNaviTransparentItemColor
                radius: 5
                Text {
                    text: "mainField5"
                    anchors.fill: parent
                    font.pixelSize: Constants.fontSize
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            GradientRectangle {
                anchors.bottom: parent.bottom
                width: 1/3 * parent.width
                height: parent.height
                text: "Custom Button"
                isCustomButton: true
                customButtonID: 2
            }
        }

        GradientRectangle {
            anchors.right: parent.right
            anchors.bottom: bottomItems.top
            anchors.bottomMargin: Constants.sdlNaviSpacing
            width: 1/6 * parent.width
            height: 1/10 * parent.height
            text: "Options"
            fontSize: Constants.fontSize
            onReleased: {
                contentLoader.go("./views/SDLPlayerOptionsListView.qml", dataContainer.currentApplication.appId)
            }
        }
    }
}
