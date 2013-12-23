/**
 * @file HardwareButton.qml
 * @brief Parent class for hardware button.
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
import com.ford.sdl.hmi.hw_buttons 1.0
import "../hmi_api/Common.js" as Common

MaskedContainer {
    property string name
    property int    buttonId: Common.ButtonName.CUSTOM_BUTTON
    property bool   upDownAvailable: true
    property bool   shortPressAvailable: true
    property bool   longPressAvailable: true

    signal hold

    Image {
        source: "../res/controlButtons/" + name + "Button.png"
    }
    Image {
        id: pressedImg
        source: "../res/controlButtons/" + name + "Button_pressed.png"
        visible: false
        Behavior on opacity {
            NumberAnimation { duration: 80 }
        }
    }
    Timer {
        id: timer
        interval: 2000
        repeat: false
        triggeredOnStart: false
    }
    property bool clickProcessed
    onPressed: {
        clickProcessed = false
        pressedImg.visible = true
        timer.start()
        if (upDownAvailable) {
            sdlButtons.onButtonEvent(buttonId, Common.ButtonEventMode.BUTTONDOWN, undefined)
        }
    }

    onReleased: {
        if (upDownAvailable) {
            sdlButtons.onButtonEvent(buttonId, Common.ButtonEventMode.BUTTONUP, undefined)
        }
        timer.stop()
        if (!clickProcessed && shortPressAvailable) {
            sdlButtons.onButtonPress(buttonId, Common.ButtonPressMode.SHORT, undefined)
        }
        pressedImg.visible = false
    }

    Connections {
        target: timer
        onTriggered: {
            if(!clickProcessed && longPressAvailable) {
                sdlButtons.onButtonPress(buttonId, Common.ButtonPressMode.LONG, undefined)
                clickProcessed = true
                hold()
            }
        }
    }

    Component.onCompleted: {
        if (buttonId !== Common.ButtonName.CUSTOM_BUTTON) {
            settingsContainer.buttonCapabilities.push(
            {
                name: buttonId,
                upDownAvailable: upDownAvailable,
                shortPressAvailable: shortPressAvailable,
                longPressAvailable: longPressAvailable
            });
        }
    }
}
