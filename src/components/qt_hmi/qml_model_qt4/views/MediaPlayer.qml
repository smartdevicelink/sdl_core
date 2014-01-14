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

import QtQuick 1.1
import "../controls"
import "../models"
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants
import "../models/Internal.js" as Internal

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

    property alias buttons: buttonsRow.content
    property alias buttonsSpacing: buttonsRow.spacing
    property alias image: image.source

    Item {
        // row of oval buttons
        id: ovalButtonsRow
        width: parent.width
        height: 1/5 * parent.height
        anchors.left: parent.left
        anchors.top: parent.top

        PagedFlickable {
            id: buttonsRow
            width: parent.width
            spacing: (mediaPlayerView.playerType === "SDL") ? ((width - 4 * elementWidth) / 3)
                                                            : (width - 2 * elementWidth)
            anchors.verticalCenter: parent.verticalCenter
            snapTo: Constants.ovalButtonWidth + spacing
            elementWidth: Constants.ovalButtonWidth
        }
    }

    Item {
        id: spacingBetweenItems
        width: parent.width
        height: 1/10 * parent.height
        anchors.left: parent.left
        anchors.top: ovalButtonsRow.bottom

        Behavior on height {
            NumberAnimation {
                duration : Constants.animationDuration
            }
        }
    }

    Column {
        // Picture + text information + media clock
        id: mediaContent
        width: parent.width
        height: 2/5 * parent.height
        anchors.left: parent.left
        anchors.top: spacingBetweenItems.bottom

        Row {
            // picture + text info
            width: parent.width
            height: 3/4 * parent.height
            spacing: Constants.margin

            Image {
                id: image
                height: parent.height
                width: height
            }

            Column {
                // text info
                id: textInfo
                height: parent.height
                width: parent.width - image.width - parent.spacing
                spacing: (height - titleText.height - 3 * text.height) / 3

                Text {
                    id: titleText
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: dataContainer.currentApplication.hmiUITextAlignment
                    color: Constants.primaryColor
                    text: (mediaPlayerView.playerType === "SDL") ? dataContainer.currentApplication.hmiUIText.mainField1
                                                                 : playerState.trackName
                    font.pixelSize: Constants.titleFontSize
                    font.bold: true
                }

                Text {
                    id: text
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: dataContainer.currentApplication.hmiUITextAlignment
                    color: Constants.primaryColor
                    text: (mediaPlayerView.playerType === "SDL") ? dataContainer.currentApplication.hmiUIText.mainField2
                                                                 : playerState.albumName
                    font.pixelSize: Constants.fontSize
                }

                Text {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: Text.AlignHCenter
                    color: Constants.primaryColor
                    text: (mediaPlayerView.playerType === "SDL") ? dataContainer.currentApplication.hmiUIText.mediaTrack
                                                                 : playerState.trackNumber
                    font.pixelSize: Constants.fontSize
                }

                Text {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: Constants.primaryColor
                    text: "Time to destination: " + dataContainer.navigationModel.timeToDestination
                    font.pixelSize: Constants.fontSize
                    horizontalAlignment: dataContainer.hmiUITextAlignment
                    visible: mediaPlayerView.playerType === "SDL" && dataContainer.navigationModel.timeToDestination
                }
            }
        }

        MediaClockView {
            width: parent.width
            height: parent.height * 1/4
        }
    }

    Item {
        id: spacingBetweenItems2
        width: parent.width
        height: 1/10 * parent.height
        anchors.left: parent.left
        anchors.top: mediaContent.bottom

        Behavior on height {
            NumberAnimation {
                duration : Constants.animationDuration
            }
        }
    }

    Row {
        // Rewind, play, pause, forward buttons
        id: playPauseRewindForward
        width: parent.width
        height: 1/5 * parent.height - statusBar.height
        anchors.left: parent.left
        anchors.leftMargin: (width - playPauseButton.width - prevButton.width - nextButton.width) / 2
        anchors.top: spacingBetweenItems2.bottom

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
            id: playPauseButton
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

    Item {
        id: presetButtons
        width: parent.width
        height: 1/5 * parent.height
        anchors.top: playPauseRewindForward.bottom
        anchors.left: parent.left

        PresetRow {
            id: presetsRow
            anchors.top: parent.top
            anchors.left: parent.left
            presets: mediaPlayerView.playerType === "SDL" ? Internal.getArrayForPresetRow() : []
            width: parent.width
            property bool clickProcessed

            Timer {
                id: timer
                interval: Constants.presetButtonTimer
                repeat: false
                triggeredOnStart: false
                onTriggered: {
                    console.log("preset button hold")
                    presetsRow.clickProcessed = true
                    sdlButtons.onButtonPress(Common.ButtonName.PRESET_0 + presetsRow.selectedIndex, Common.ButtonPressMode.LONG, undefined)
                }
            }

            onPresetButtonPressed: {
                console.log("preset button pressed")
                timer.start()
                clickProcessed  = false
                sdlButtons.onButtonEvent(Common.ButtonName.PRESET_0 + selectedIndex, Common.ButtonEventMode.BUTTONDOWN, undefined)
            }

            onPresetButtonReleased: {
                console.log("preset button released")
                sdlButtons.onButtonEvent(Common.ButtonName.PRESET_0 + selectedIndex, Common.ButtonEventMode.BUTTONUP, undefined)
                timer.stop()
                if (!clickProcessed) {
                    sdlButtons.onButtonPress(Common.ButtonName.PRESET_0 + selectedIndex, Common.ButtonPressMode.SHORT, undefined)
                }
            }
        }
    }

    StatusBar {
        id: statusBar
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }

    states: [
        State {
            name: 'presetButtonsON'
            when: (dataContainer.currentApplication.customPresets.count > 0) && (mediaPlayerView.playerType === "SDL")
            PropertyChanges {
                target: spacingBetweenItems
                height: 1
            }
            PropertyChanges {
                target: spacingBetweenItems2
                height: 1
            }
            PropertyChanges {
                target: presetButtons
                visible: true
                enabled: true
            }
        },

        State {
            name: 'presetButtonsOFF'
            when: (dataContainer.currentApplication.customPresets.count === 0) || (mediaPlayerView.playerType !== "SDL")
            PropertyChanges {
                target: spacingBetweenItems
                height: 1/10 * mediaPlayerView.height
            }
            PropertyChanges {
                target: spacingBetweenItems2
                height: 1/10 * mediaPlayerView.height
            }
            PropertyChanges {
                target: presetButtons
                visible: false
                enabled: false
            }
        }
    ]
}
