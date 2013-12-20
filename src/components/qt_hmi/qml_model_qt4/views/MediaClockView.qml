/**
 * @file MediaClockView.qml
 * @brief Media clock view
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
import "../models/Constants.js" as Constants
import "../models/Internal.js" as Internal

Item {
    property alias time: timeText.text
    Text {
        id: timeText
        anchors.left: parent.left
        width: 1/10 * parent.width
        anchors.verticalCenter: parent.verticalCenter
        horizontalAlignment: Text.AlignRight
        color: "white"
        text: (mediaPlayerView.playerType === "SDL") ? Internal.hmsTimeToString(dataContainer.currentApplication.mediaClock.startTime)
                                                     : "02:36" //TODO {Aleshin}: get track time for all players except SDL
        font.pixelSize: 18
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height
        width: 2/3 * parent.width

        Rectangle {
           anchors.verticalCenter: parent.verticalCenter
           height: 2
           width: dataContainer.currentApplication.mediaClock.progress * parent.width
           color: "white"
        }

        Rectangle {
           anchors.verticalCenter: parent.verticalCenter
           height: 2
           width: (1 - dataContainer.currentApplication.mediaClock.progress) * parent.width
           color: Constants.primaryColor
        }
    }
}
