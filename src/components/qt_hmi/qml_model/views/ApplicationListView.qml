/**
 * @file ApplicationListView.qml
 * @brief Application list view
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
import "../models"
import "../models/Constants.js" as Constants

GeneralView {
    Column {
        anchors.fill: parent
        Item {
            // top 1/4 of screen
            width: parent.width
            height: 1/4 * parent.height

            PagedFlickable {
                width: parent.width
                snapTo: 200 + spacing
                spacing: (width - 600) / 2
                LongOvalButton {
                    width: 200
                    text: "Vehicle Health Report"
                }
                LongOvalButton {
                    width: 200
                    text: "911 Assist"
                }
                LongOvalButton {
                    width: 200
                    text: "Install applications/Up"
                }
                LongOvalButton {
                    width: 200
                    text: "Find New Apps"
                }
                LongOvalButton {
                    width: 200
                    text: "Change device"
                    dest: "./views/ChangeDeviceView.qml"
                }
            }
        }

        Item {
            height: parent.height / 2
            width: parent.width

            ListView {
                id: applicationListView
                anchors.fill: parent
                model: dataContainer.applicationList

                anchors.horizontalCenter: parent.horizontalCenter
                width:parent.width
                height:parent.height

                delegate: Row {
                    width: parent.width
                    height: appName.height
                    Image { id: sdlIcon; source: icon }
                    ClickableText {
                        text: appName
                        defaultColor: Constants.primaryColor
                        pressedColor: Constants.primaryColorPressed
                        font.pixelSize: Constants.titleFontSize
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            dataContainer.setCurrentApplication(appId)
                            dataContainer.currentApplication.isMediaApplication ? contentLoader.go("./views/SDLPlayerView.qml", dataContainer.currentApplication.appId)
                                                                                : contentLoader.go("./views/SDLNonMediaView.qml", dataContainer.currentApplication.appId)
                        }
                    }
                }
            }
        }

        Item {
            // bottom 1/4 of screen
            width: parent.width
            height: 1/4 * parent.height

            BackButton { anchors.centerIn: parent}
        }
    }
}
