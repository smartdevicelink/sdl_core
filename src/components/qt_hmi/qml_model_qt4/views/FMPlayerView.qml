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

import QtQuick 1.1
import "../controls"
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants

GeneralView {
    category: Common.DeactivateReason.AUDIO
    RadioPlayer {
        id: fmRadioPlayer
        anchors.fill: parent
        radioType: "FM"
        radioName: "FM Radio"

        playerState: dataContainer.fmPlayerState

        property int hdButtonValue: 1

        buttonHD: [
            Image {
                id: hdButton
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                source: "../res/buttons/long_oval_btn.png"
                property string textColor: Constants.primaryColor

                Row {
                    anchors.centerIn: parent
                    spacing: (hdButton.width - hdLogo.width - one.width - two.width - three.width - four.width) / 10

                    Image {
                        id: hdLogo
                        anchors.verticalCenter: parent.verticalCenter
                        source:"../res/hd_logo_on.png"
                    }

                    Text {
                        id: one
                        anchors.verticalCenter: parent.verticalCenter
                        text: "1"
                        color: fmRadioPlayer.hdButtonValue === 1 ? "white" : hdButton.textColor
                        font.pixelSize: Constants.fontSize
                    }

                    Text {
                        id: two
                        anchors.verticalCenter: parent.verticalCenter
                        text: "2"
                        color: fmRadioPlayer.hdButtonValue === 2 ? "white" : hdButton.textColor
                        font.pixelSize: Constants.fontSize
                    }

                    Text {
                        id: three
                        anchors.verticalCenter: parent.verticalCenter
                        text: "3"
                        color: fmRadioPlayer.hdButtonValue === 3 ? "white" : hdButton.textColor
                        font.pixelSize: Constants.fontSize
                    }

                    Text {
                        id: four
                        anchors.verticalCenter: parent.verticalCenter
                        text: "4"
                        color: fmRadioPlayer.hdButtonValue === 4 ? "white" : hdButton.textColor
                        font.pixelSize: Constants.fontSize
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        hdLogo.source = "../res/hd_logo_off.png"
                        hdButton.source = "../res/buttons/long_oval_pressed_btn.png"
                        hdButton.textColor = Constants.secondaryColor

                    }
                    onReleased: {
                        hdLogo.source = "../res/hd_logo_on.png"
                        hdButton.source = "../res/buttons/long_oval_btn.png"
                        hdButton.textColor = Constants.primaryColor
                        fmRadioPlayer.hdButtonValue === 4 ? fmRadioPlayer.hdButtonValue = 1 : fmRadioPlayer.hdButtonValue++
                    }
                }
            }
        ]
    }
}
