/**
 * @file RadioPlayerView.qml
 * @brief Parent for AM, FM, Sirius players screen view.
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
    id: radioPlayerView

    property string radioType: ""
    property string radioName: ""
    property string songName: ""
    property string albumName: ""
    property var presets: []

    property alias buttonHD: bot.children

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
                text: radioName
                pixelSize: 20
                dest: "./views/MusicSourceGridView.qml"
            }

            LongOvalButton {
                anchors.right: parent.right
                anchors.top: parent.top
                text: "Tune"
                pixelSize: 20
            }
        }

        Item {
            // mid part for information about song
            id: mid
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            width: parent.width

            Column {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left

                Row{
                    Text {
                        id: radioChannelNameText
                        color: "#1d81d5"
                        text: presets[0]
                        font.pixelSize: 45
                    }

                    Text {
                        anchors.bottom: radioChannelNameText.bottom
                        color: "#1d81d5"
                        text: " " + radioType
                        font.pixelSize: 25
                    }
                }

                Text {
                    color: "#1d81d5"
                    text: songName
                    font.pixelSize: 25
                    font.bold: true
                }

                Text {
                    color: "#1d81d5"
                    text: albumName
                    font.pixelSize: 25
                }
            }
        }

        Item {
            // bottom part for HD button (for FM radio)
            id: bot
            anchors.left: parent.left
            anchors.bottom: parent. bottom
            width: parent.width
            height: parent.height * 1/4
        }
    }

    Item {
        // bottom 1/4 screen
        id: lowerContent
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        // Line that divide screen in two parts
        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 2
            color: "#1d81d5"
        }

        PresetRow{
            anchors.centerIn: parent
            presets: radioPlayerView.presets
            width: parent.width
            onSelectedIndexChanged: {
                radioChannelNameText.text = presets[selectedIndex];
            }
        }
    }
}


