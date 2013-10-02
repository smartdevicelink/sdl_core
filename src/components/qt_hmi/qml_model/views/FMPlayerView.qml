/**
 * @file FMPlayerView.qml
 * @brief FM player screen view.
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

GeneralView {
    RadioPlayerView {
        anchors.fill: parent

        radioType: "FM"
        presets: ["96.3", "107.9", "104.3", "101.9", "105.3", "100.5", "107.9", "103.4"]

        LongOvalButton {
            id: hdBtn
            Row {
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
}
