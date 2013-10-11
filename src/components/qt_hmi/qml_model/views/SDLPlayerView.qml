/**
 * @file SDLPlayerView.qml
 * @brief SDL player screen view.
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
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants

GeneralView {
    applicationContext: true
    MediaPlayer {
        onPlay: { sdlButtons.onButtonPress(Common.ButtonName.OK, Common.ButtonPressMode.SHORT, undefined) }
        onPause: { sdlButtons.onButtonPress(Common.ButtonName.OK, Common.ButtonPressMode.SHORT, undefined) }
        playerName: "SDL music"
        anchors.fill: parent

        playerState: dataContainer.sdlPlayerState

        topOvalButtons: [
            Row {
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: (parent.width - 4 * longOvalButton.width) / 3
                width: 2 * longOvalButton.width + spacing

                LongOvalButton {
                    id: longOvalButton
                    text: "SDL Menu"
                    pixelSize: Constants.fontSize
                    dest: "./views/AppsMenuGridView.qml"
                }
                LongOvalButton {
                    text: "Options"
                    pixelSize: Constants.fontSize
                    dest: "./views/SDLPlayerOptionsListView.qml"
                }
            }
        ]
    }
}
