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
import "../models"
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants

Item {
    id: mediaPlayerView

    property string playerName: ""
    property string playerType: ""

    signal rewind
    signal forward
    signal play
    signal pause

    // Holds players state(song name, play/pause state, track number etc). For all players except SDL.
    property PlayerState playerState;

    property alias buttons: flickRow.content

    Item {
        // top 3/4 screen
        id: upperContent
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left

        Item {
            id: top
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: parent.height * 1/4

            PagedFlickable {
                id: flickRow
                width: parent.width
                spacing: (mediaPlayerView.playerType === "SDL") ? ((width - longOvalButton.width * 4) / 3)
                                                                : (width - longOvalButton.width * 2)
                snapTo: longOvalButton.width + spacing
                elementWidth: longOvalButton.width
            }
        }

        Item {
            // mid part for picture, information about song, album
            id: mid
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            width: parent.width
            height: parent.height * 2/4

            Image {
                id: image
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                source: (mediaPlayerView.playerType === "SDL") ? dataContainer.currentApplication.hmiUIText.picture //TODO {ALeshin}: get picture correctly
                                                               : playerState.albumImage
            }

            Item {
                id: space
                anchors.left: image.right
                width: 20
            }

            Column {
                anchors.left: space.right
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                spacing: parent.height / 5

                Text {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: dataContainer.hmiUITextAlignment
                    color: Constants.primaryColor
                    text: (mediaPlayerView.playerType === "SDL") ? dataContainer.currentApplication.hmiUIText.mainField1
                                                                 : playerState.trackName
                    font.pixelSize: 45
                    font.bold: true
                }

                Text {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: dataContainer.hmiUITextAlignment
                    color: Constants.primaryColor
                    text: (mediaPlayerView.playerType === "SDL") ? dataContainer.currentApplication.hmiUIText.mainField2
                                                                 : playerState.albumName
                    font.pixelSize: 25
                }

                Text {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: Constants.primaryColor
                    text: (mediaPlayerView.playerType === "SDL") ? dataContainer.currentApplication.hmiUIText.mainField3
                                                                 : playerState.trackNumber
                    font.pixelSize: 20
                }
            }
        }

        MediaClockView {
            anchors.left: parent.left
            anchors.bottom: parent. bottom
            width: parent.width
            height: parent.height * 1/4
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
            id: mediaControl

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
                anchors.verticalCenter: parent.verticalCenter
                state: (mediaPlayerView.playerType === "SDL") ? dataContainer.currentApplication.playPauseState : playerState.playPauseState
                onClicked: {
                    (state == 'Play') ? play() : pause();
                    var newState = state === "Play" ? "Pause" : "Play";
                    (mediaPlayerView.playerType === "SDL") ? dataContainer.setApplicationProperties(dataContainer.currentApplication.appId, { playPauseState: newState } )
                                                           : playerState.playPauseState = newState
                }
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

        Text {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: mediaControl.bottom
            anchors.bottom: parent.bottom
            text: (mediaPlayerView.playerType === "SDL") ? dataContainer.currentApplication.hmiUIText.statusBar
                                                         : ""
            color: Constants.primaryColor
        }
    }
}
