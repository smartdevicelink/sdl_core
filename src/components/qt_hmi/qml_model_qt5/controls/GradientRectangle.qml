/**
 * @file GradientRectangle.qml
 * @brief Rectangle with gradient.
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
import "../models/Constants.js" as Constants
import "../hmi_api/Common.js" as Common

Rectangle {
    property string firstColor: "grey"
    property string secondColor: "#2E2E2E"
    property alias fontSize: text.font.pixelSize
    property alias text: text.text
    property int customButtonID
    property bool isCustomButton: false
    property bool clickProcessed
    signal pressed()
    signal released()

    radius: 5
    border.color: "#D3D3D3"

    Text {
        id: text
        anchors.fill: parent
        font.pixelSize: Constants.titleFontSize
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            firstColor = "#2E2E2E"
            secondColor = "grey"
            parent.pressed()
        }
        onReleased: {
            firstColor = "grey"
            secondColor = "#2E2E2E"
            parent.released()
        }
    }

    Timer {
        id: timer
        interval: Constants.customButtonTimer
        repeat: false
        triggeredOnStart: false
    }

    onPressed: {
        if (isCustomButton) {
            timer.start()
            clickProcessed  = false
            sdlButtons.onButtonEvent(Common.ButtonName.CUSTOM_BUTTON, Common.ButtonEventMode.BUTTONDOWN, customButtonID)
        }
    }

    onReleased: {
        if (isCustomButton) {
            sdlButtons.onButtonEvent(Common.ButtonName.CUSTOM_BUTTON, Common.ButtonEventMode.BUTTONUP, customButtonID)
            timer.stop()
            if (!clickProcessed) {
                sdlButtons.onButtonPress(Common.ButtonName.CUSTOM_BUTTON, Common.ButtonPressMode.SHORT, customButtonID)
            }
        }
    }

    Connections {
        target: timer
        onTriggered: {
            if (isCustomButton) {
                if(!clickProcessed) {
                    sdlButtons.onButtonPress(Common.ButtonName.CUSTOM_BUTTON, Common.ButtonPressMode.LONG, customButtonID)
                    clickProcessed = true
                }
            }
        }
    }

    gradient: Gradient {
        GradientStop {
            position: 0.1
            color: firstColor
        }
        GradientStop {
            position: 0.5
            color: secondColor
        }
    }
}
