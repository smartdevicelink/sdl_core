/**
 * @file MediaPlayerView.qml
 * @brief Parent for BT, IPod, CD players screen view.
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
    id: mediaPlayerView

    property string playerName: ""
    property string albumImage: ""
    property string trackNumber: ""
    property string trackName: ""
    property string albumName: ""

    property alias topOvalButtons: top.children

    signal rewind
    signal forward
    signal play
    signal pause

    Item {
        // top 3/4 screen
        id: upperContent
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left

        Item {
            // top part for buttons
            id: top
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: parent.height * 1/4

            LongOvalButton {
                anchors.left: parent.left
                anchors.top: parent.top
                text: playerName
                pixelSize: 20
                dest: "./views/MusicSourceView.qml"
            }

            LongOvalButton {
                anchors.right: parent.right
                anchors.top: parent.top
                text: "Browse"
                pixelSize: 20
            }
        }

        Item {
            // mid part for picture, information about song, album
            id: mid
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            width: parent.width
            height: parent.height * 2/4

            Row {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                width: parent.width
                spacing: 20

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.height
                    height: parent.height
                    source: albumImage
                }

                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: parent.height / 5

                    Text {
                        color: "#1d81d5"
                        text: "Track " + trackNumber
                        font.pixelSize: 20
                    }

                    Text {
                        color: "#1d81d5"
                        text: trackName
                        font.pixelSize: 45
                        font.bold: true
                    }

                    Text {
                        color: "#1d81d5"
                        text: albumName
                        font.pixelSize: 25
                    }
                }
            }
        }

        Item {
            // bottom part
            id: bot
            anchors.left: parent.left
            anchors.bottom: parent. bottom
            width: parent.width
            height: parent.height * 1/4

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
        //bottom 1/4 screen
        id: lowerContent
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        Row {
            anchors.centerIn: parent
            Image {
                id: prevButton
                anchors.verticalCenter: parent.verticalCenter
                source: "../res/buttons/player_prev_btn.png"
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        prevButton.source = "../res/buttons/player_prev_pressed_btn.png"
                    }
                    onReleased: {
                        prevButton.source = "../res/buttons/player_prev_btn.png"
                    }
                }
            }

            PlayPauseButton {
                onClicked: { (state == 'Play') ? pause() : play() }
            }

            Image {
                id: nextButton
                anchors.verticalCenter: parent.verticalCenter
                source: "../res/buttons/player_next_btn.png"
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        nextButton.source = "../res/buttons/player_next_pressed_btn.png"
                    }
                    onReleased: {
                        nextButton.source = "../res/buttons/player_next_btn.png"
                    }
                }
            }
        }
    }
}
