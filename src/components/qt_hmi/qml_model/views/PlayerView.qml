/**
 * @file PlayerView.qml
 * @brief BT, IPod, CD player view screen.
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

Item {
    id: playerView
    anchors.fill: parent
    property string playerType: ""

    Item {
        // 3/4 top screen
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        LongOvalButton {
            text: playerType
            pixelSize: 20
            dest: "./views/MusicSourceGridView.qml"
            anchors.left: parent.left
            anchors.top: parent.top
        }

        LongOvalButton {
            text: "Browse"
            pixelSize: 20
            anchors.right: parent.right
            anchors.top: parent.top
        }

        // Middle row with picture and text near it
        Row {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 1/20 * parent.width
            spacing: albumArtImage.width * 1/3

            Image {
                id: albumArtImage
                source: "../res/album_art.png"
            }

            Column {
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    color: "#1d81d5"
                    text: "Track 13 / 16"
                    font.pixelSize: 20
                }

                Text {
                    color: "#1d81d5"
                    text: "Song name"
                    font.pixelSize: 45
                    font.bold: true
                }

                Text {
                    color: "#1d81d5"
                    text: "Album Name"
                    font.pixelSize: 25
                }
            }
        }

        Item {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: parent.height * 1/10

            Text {
                id: time
                anchors.left: parent.left
                anchors.leftMargin: 1/10 * parent.width
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                text: "02:36"
                font.pixelSize: 18
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                width: 2/3 * parent.width

                Rectangle {
                   id: firstRect
                   anchors.verticalCenter: parent.verticalCenter
                   height: 2
                   width: 1/3 * parent.width
                   color: "white"
                }

                Rectangle {
                   id: secondRect
                   anchors.verticalCenter: parent.verticalCenter
                   height: 2
                   width: 2/3 * parent.width
                   color: "#1d81d5"
                }
            }
                Text {
                    anchors.right: parent.right
                    anchors.rightMargin: 1/10 * parent.width
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#1d81d5"
                    text: "04:23"
                    font.pixelSize: 18
                }
        }
    }

    Item {
        // 1/4 bottom screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        Row {

            anchors.centerIn: parent
            Image {
                id: prevBtnImage
                anchors.verticalCenter: parent.verticalCenter
                source: "../res/buttons/player_prev_btn.png"
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        prevBtnImage.source = "../res/buttons/player_prev_pressed_btn.png"
                    }
                    onReleased: {
                        prevBtnImage.source = "../res/buttons/player_prev_btn.png"
                    }
                }
            }

            PlayPauseButton {  }

            Image {
                id: nextBtnImage
                anchors.verticalCenter: parent.verticalCenter
                source: "../res/buttons/player_next_btn.png"
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        nextBtnImage.source = "../res/buttons/player_next_pressed_btn.png"
                    }
                    onReleased: {
                        nextBtnImage.source = "../res/buttons/player_next_btn.png"
                    }
                }
            }
        }
    }
}
