/**
 * @file RadioPlayerView.qml
 * @brief Parent for AM, FM players screen view.
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
    id: radioPlayer
    anchors.fill: parent
    property string radioType: ""
    property var activeBand: radioType === "AM"? ["1130", "950", "760", "1270"]: ["96.3", "107.9", "104.3", "101.9"]
    property var presets: [ "1130", "950", "760", "1270", "96.3", "107.9", "104.3", "101.9" ]
    //property int minHeight: 400

    Item {
        // 3/4 top screen
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left

        // Buttons at top
        Item {
            anchors.top: parent.top
            anchors.left: parent.left
            height: parent.height * 1/4
            width: parent.width

            LongOvalButton {
                anchors.left: parent.left
                anchors.top: parent.top
                text: radioType + " Radio"
                pixelSize: 20
                dest: "./views/MusicSourceGridView.qml"
            }

            LongOvalButton {
                anchors.top: parent.top
                anchors.right: parent.right
                text: "Tune"
                pixelSize: 20
            }
        }


        // Text information
        Item {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height * 2/4
            width: parent.width

            Column {
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10
                Row{

                    Text {
                        id: radioChannelNameText
                        color: "#1d81d5"
                        text: presets[0]
                        font.pixelSize: 45
                    }

                    Text {
                        color: "#1d81d5"
                        text: " " + radioType
                        font.pixelSize: 25
                        anchors.bottom: radioChannelNameText.bottom
                    }
                }

                Text {
                    color: "#1d81d5"
                    text: "Song name"
                    font.pixelSize: 25
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
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            height: parent.height * 1/4
            width: parent.width

            LongOvalButton {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                id: hdBtn
                visible: radioPlayer.radioType == "FM"
                Row{
                    id: row
                    anchors.verticalCenter: parent.verticalCenter
                    x: parent.x + 20
                    spacing: 5
                    property int selected: 0
                    MouseArea {
                        onClicked: {
                            //todo: change color of text
                        }
                    }

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        source:"../res/hd_logo_on.png"
                    }
                    Text {
                        id: one
                        anchors.verticalCenter: parent.verticalCenter
                        text: "1"
                        color: "white"
                        font.pixelSize: 25
                    }
                    Text {
                        id: two
                        anchors.verticalCenter: parent.verticalCenter
                        text: "2"
                        color: parent.selected == 1 ? "white" : "#1d81d5"
                        font.pixelSize: 25
                    }
                    Text {
                        id: three
                        anchors.verticalCenter: parent.verticalCenter
                        text: "3"
                        color: parent.selected == 2 ? "white" : "#1d81d5"
                        font.pixelSize: 25
                    }
                    Text {
                        id: fourth
                        anchors.verticalCenter: parent.verticalCenter
                        text: "4"
                        color: parent.selected == 3 ? "white" : "#1d81d5"
                        font.pixelSize: 25
                    }
                }
            }
        }

        Rectangle{
            anchors.bottom: parent.bottom
            width: parent.width
            height: 2
            color: "#1d81d5"
        }
    }

    Item {
        // 1/4 bottom screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        PresetRow {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            presets: radioPlayer.presets
            onSelectedIndexChanged: {
                radioChannelNameText.text = presets[selectedIndex];
            }
        }
    }
}


