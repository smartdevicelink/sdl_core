/**
 * @file PushButton.qml
 * @brief Simple button
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

Rectangle {
    id: toggleButton
    width: 160
    height: 40
    radius: 2
    border.width: 2
    border.color: "#000000"
    gradient: grUnpressed

    property alias label : label.text

    signal pressed()
    signal unpressed()
    signal clicked()

    property bool toggleMode: false

    state: "unpressed"
    onStateChanged: {
        if (state == "pressed") {
            pressed();
        } else {
            unpressed();
        }
    }

    Text {
        id: label
        color: "white"
        text: "PushButton"
        font.pixelSize: 18
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.fill: parent
    }
    MouseArea {
        id: mouseArea

        anchors.fill: parent
        onPressed: {
            if (toggleMode) {
                if (parent.state == "unpressed") {
                    parent.state = "pressed"
                } else {
                    parent.state = "unpressed"
                }
            } else {
                parent.state = "pressed"
            }
        }
        onReleased: {
            if (!toggleMode) {
                parent.state = "unpressed"
                if (mouse.isClick) {
                    parent.clicked()
                }
            }
        }
    }

    states: [
        State  {
            name: "pressed"
            PropertyChanges  {
                target: toggleButton
                gradient: grPressed
            }
        },
        State  {
            name: "unpressed"
            PropertyChanges  {
                target: toggleButton
                gradient: grUnpressed
            }
        }
    ]

    Gradient {
        id: grUnpressed

        GradientStop { position: 0.0; color: "#2c2c2c" }
        GradientStop { position: 1.0; color: "#0c0c0c" }
    }

    Gradient {
        id: grPressed

        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 1.0; color: "black" }
    }

    transitions: [
        Transition  {
            from: "unpressed"
            to: "pressed"
            reversible: true
        }
    ]
}
