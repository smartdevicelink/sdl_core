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

    property var presets: []
    property int minHeight: 400
    property string radioType

    default property alias content: additional.children

    Column {
        anchors.fill: parent

        Item {
            id: upperControlLine
            width: parent.width
            height: parent.height / 4
            anchors.horizontalCenter: parent.horizontalCenter
            LongOvalButton {
                text: radioType + " Radio"
                pixelSize: 20
                dest: "./views/MusicSourceGridView.qml"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }

            LongOvalButton {
                text: "Tune"
                pixelSize: 20
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Item {
            width: parent.width
            height: parent.height / 2

            Column {
                spacing: 10
                Row {
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
                Row {
                    Text {
                        color: "#1d81d5"
                        text: "Song name"
                        font.pixelSize: 25
                        font.bold: true
                    }
                }
                Row {
                    Text {
                        color: "#1d81d5"
                        text: "Album Name"
                        font.pixelSize: 25

                    }
                }
                Item {
                    width: childrenRect.width
                    height: childrenRect.height
                    id: additional
                }
            }
        }


        Item {
            width: parent.width
            height: parent.height / 4
            Rectangle {
                width: parent.width
                height: 2
                color: "#1d81d5"
            }
            PresetRow {
                anchors.centerIn: parent
                anchors.bottom: parent.bottom
                presets: radioPlayer.presets
                width: parent.width
                onSelectedIndexChanged: {
                    radioChannelNameText.text = presets[selectedIndex];
                }
            }
        }
    }
}


