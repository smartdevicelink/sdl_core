/**
 * @file CircleButton.qml
 * @brief Parent class for circle button.
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

Image {
    id: circleBtn
    source: imgOff
    property alias text: btnText.text
    property alias pixelSize: btnText.font.pixelSize
    property string  dest: ""
    property bool isPressed: false
    property string imgOff: "../res/buttons/round_btn.png"
    property string imgOn: "../res/buttons/round_pressed_btn.png"
    property string textColorOnPressed: Constants.secondaryColor
    property string textColorDefault: Constants.primaryColor

    signal clicked()
    function wasClicked()
    {
        clicked()
    }

    Text {
        anchors.centerIn: parent
        id: btnText
        color: textColorDefault
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            console.log("CircleButton onPressed enter");
            source = imgOn
            btnText.color = textColorOnPressed
            isPressed = true
            console.log("CircleButton onPressed exit");
        }
        onReleased: {
            console.log("CircleButton onReleased enter");
            source = imgOff
            btnText.color =  textColorDefault
            isPressed = false
            console.log("CircleButton onReleased exit");
        }
        onClicked: {
            console.log("CircleButton onClicked enter");
            if(dest !== ""){
                contentLoader.go(dest)
            }
            circleBtn.wasClicked()
            console.log("CircleButton onClicked enter");
        }
    }
}
