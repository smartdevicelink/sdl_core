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

import QtQuick 1.1
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
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width
                elementWidth: Constants.ovalButtonWidth
                snapTo: elementWidth + spacing
                spacing: (width - 4 * elementWidth) / 3

                OvalButton {
                    text: "Find New Apps"
                }

                OvalButton {
                    text: "Change device"
                    onReleased: contentLoader.go("./views/ChangeDeviceView.qml")
                }

                OvalButton {
                    text: "911 Assist"
                }

                OvalButton {
                    text: "Vehicle Health Report"
                }

                OvalButton {
                    text: "Install applications/Up"
                }
            }
        }

        Item {
            height: parent.height / 2
            width: parent.width

            ScrollableListView {
                id: applicationListView
                anchors.fill: parent
                model: dataContainer.applicationList

                delegate: Item {
                    width: parent.width
                    height: Math.max(applicationName.height, appIcon.height)
                    Image {
                        id: appIcon
                        source: icon
                        height: Constants.appListIconSize
                        width: height
                    }
                    ClickableText {
                        id: applicationName
                        text: appName
                        defaultColor: Constants.primaryColor
                        pressedColor: Constants.primaryColorPressed
                        font.pixelSize: Constants.appListFontSize
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: appIcon.right
                        anchors.leftMargin: Constants.margin
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

            BackButton { anchors.centerIn: parent }
        }
    }
}
