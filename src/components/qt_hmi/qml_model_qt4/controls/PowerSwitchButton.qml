/**
 * @file PowerSwitchBtn.qml
 * @brief On/Off button.
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

Rectangle {
    id: powerSwitchBtn
    state: "Turn ON"
    width: 120
    height: 80
    radius: 5
    property bool pressed;

    gradient: Gradient {
        GradientStop { position: pressed ? 1.0 : 0.0; color: "#2c2c2c" }
        GradientStop { position: pressed ? 0.0 : 1.0; color: "black" }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            parent.pressed = true
        }
        onReleased: {
            parent.pressed = false
            parent.state = parent.state === "Turn ON" ? "Turn OFF" : "Turn ON"
            btnText.text = parent.state
            if (parent.state === "Turn OFF") {
                contentLoader.reset()
                mainScreen.visible = true
                warningInfo.showOkButton()
            }
        }
    }

    Text {
        id: btnText
        color: "red"
        text: parent.state
        anchors.centerIn: parent
        font.pixelSize: Constants.powerButtonFornSize
    }
}
