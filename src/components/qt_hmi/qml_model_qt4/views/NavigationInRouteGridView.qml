/**
 * @file NavigationInRouteGridView.qml
 * @brief Navigation in route screen veiw.
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
import "../models/Constants.js" as Constants


GeneralView {
    Item {
        id: navInRoute
        anchors.fill: parent

        Image {
            // 3/4 top screen
            id: map
            height: parent.height * 3/4
            width: parent.width
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            source: "../res/nav/map.png"

            Image {
                id: compas
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 1/4 * compas.width
                anchors.left: parent.left
                anchors.leftMargin: 1/4 * compas.width
                source: "../res/nav/compass.png"
            }

            Column {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: compas.horizontalCenter
                spacing: 1/8 * parent.height

                // Zoom "+" button
                Image {
                    id: zoomIn
                    source: "../res/buttons/zoom.png"

                    Text {
                        id: zoomInBtn
                        anchors.centerIn: parent
                        text: "+"
                        color: Constants.releasedButtonTextColor
                        font.pixelSize: 30
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            parent.source = "../res/buttons/zoom_pressed.png"
                            zoomInBtn.color = Constants.pressedButtonTextColor
                        }
                        onReleased:  {
                            parent.source = "../res/buttons/zoom.png"
                            zoomInBtn.color = Constants.releasedButtonTextColor
                        }

                        onClicked: {
                            //Some behavior
                        }
                    }
                }

                // Zoom "-" button
                Image {
                    id: zoomOut
                    source: "../res/buttons/zoom.png"

                    Text {
                        id: zoomOutBtn
                        anchors.centerIn: parent
                        text: "-"
                        color: "white"
                        font.pixelSize: 30
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            parent.source = "../res/buttons/zoom_pressed.png"
                            zoomOutBtn.color = Constants.pressedButtonTextColor
                        }
                        onReleased:  {
                            parent.source = "../res/buttons/zoom.png"
                            zoomOutBtn.color = Constants.releasedButtonTextColor
                        }

                        onClicked: {
                            //Some behavior
                        }
                    }
                }
            } // column

            // Options button
            Image {
                id: navOptions
                anchors.top: parent.top
                anchors.right: parent.right
                source: "../res/nav/options.png"

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        parent.source = "../res/nav/options_pressed.png"
                    }
                    onReleased:  {
                        parent.source = "../res/nav/options.png"
                    }
                    onClicked: {
                    //Options screen
                    }
                }
            }
        }   //map

        Item {
            // 1/4 bottom screen
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width
            height: 1/4 * parent.height

            Image {
               id: muteBtnImg
               anchors.verticalCenter: parent.verticalCenter
               anchors.left: parent.left
               anchors.leftMargin: 1/4 * muteBtnImg.width
               source: "../res/nav/mute_off.png"
                    MouseArea {
                      anchors.fill: parent
                      onClicked: if (muteBtnImg.state === "off") {
                                       muteBtnImg.state = "on"
                                       muteBtnImg.source = "../res/nav/mute_off.png"
                                 }
                                 else {
                                     muteBtnImg.source = "../res/nav/mute_on.png"
                                     muteBtnImg.state = "off"
                                 }
                    }
             }

            Image {
                id: turnArrow
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: muteBtnImg.right
                anchors.leftMargin: 1/20 * contentLoader.width
                source: "../res/nav/turnArrow.png"
            }

            Text {
                id: street
                anchors.verticalCenter: cancel.verticalCenter
                anchors.left: turnArrow.right
                anchors.leftMargin: 1/20 * contentLoader.width
                text: "0.2 mi on Cherry Hill Rd."
                color: "White"
                font.pixelSize: 20
            }

            Text {
                id: timeToDest
                anchors.verticalCenter: cancel.verticalCenter
                anchors.right: cancel.left
                anchors.rightMargin: 1/20 * contentLoader.width
                text: "2 hrs 27 min"
                color: "White"
                font.pixelSize: 20
            }

            Image {
                id: cancel
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right

                source: "../res/buttons/long_oval_btn.png"

                Image {
                    id: destIcon
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: cancel.left
                    anchors.leftMargin: Constants.fontSize
                    source: "../res/nav/dest_icon.png"
                }

                Text {
                    id: cancelText
                    anchors.centerIn: parent
                    text: "Cancel"
                    color: Constants.primaryColor
                    font.pixelSize: 20
                }

                MouseArea {
                    anchors.fill: parent

                    onPressed: {
                        cancel.source = "../res/buttons/long_oval_pressed_btn.png"
                        destIcon.source = "../res/nav/dest_icon_black.png"
                        cancelText.color = Constants.pressedButtonTextColor
                    }

                    onReleased: {
                        cancel.source = "../res/buttons/long_oval_btn.png"
                        destIcon.source = "../res/nav/dest_icon.png"
                        cancelText.color = Constants.primaryColor
                    }

                    onClicked: {
                        contentLoader.go("./views/NavigationNoRouteGridView.qml")
                    }
                }
            }
        }
    }
}
