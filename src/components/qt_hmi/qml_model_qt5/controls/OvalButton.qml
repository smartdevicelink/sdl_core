/**
 * @file OvalButton.qml
 * @brief Oval button with flexible width.
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
import QtGraphicalEffects 1.0
import "../models/Constants.js" as Constants

// Don't change constants. It break button
// TODO (dchmerev@luxoft.com): make this comment more clear
// todo (ykazakov): eliminate this problem and remove all such comments

Item {
    id: main
    width: dynamic ? field.width + 2 * left.width : Constants.ovalButtonWidth
    height: Constants.ovalButtonHeight

    signal clicked;
    signal pressed;
    signal released;
    signal pressAndHold;
    property alias text: label.text
    property alias fontSize: label.font.pixelSize
    property alias icon: image.source
    property bool highlighted: false
    property bool dynamic: false
    property bool disabled: false

    function hue() {
        var ret = 0;
        if (disabled) {
            ret = Constants.disabledButtonHue;
        } else if (highlighted) {
            ret = Constants.highlightedButtonHue;
        }
        return ret;
    }

    function saturation() {
        var ret = 0;
        if (disabled) {
            ret = Constants.disabledButtonSaturation;
        } else if (highlighted) {
            ret = Constants.highlightedButtonSaturation;
        }
        return ret;
    }

    function lightness() {
        var ret = 0;
        if (disabled) {
            ret = Constants.disabledButtonLightness;
        } else if (highlighted) {
            ret = Constants.highlightedButtonLightness;
        }
        return ret;
    }

    Item {
        id: buttonBorderImage
        visible: false
        anchors.fill: parent
        Image {
            id: left
            width: 31
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            source: "../res/buttons/oval_btn_left.png"
        }

        Image {
            id: right
            width: 31
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            source: "../res/buttons/oval_btn_right.png"
        }

        Image {
            id: top
            height: 10
            anchors.right: parent.right
            anchors.rightMargin: 31
            anchors.left: parent.left
            anchors.leftMargin: 31
            anchors.top: parent.top
            fillMode: Image.TileHorizontally
            source: "../res/buttons/oval_btn_top.png"
        }

        Image {
            id: bottom
            height: 11
            anchors.right: parent.right
            anchors.rightMargin: 31
            anchors.left: parent.left
            anchors.leftMargin: 31
            anchors.bottom: parent.bottom
            fillMode: Image.TileHorizontally
            source: "../res/buttons/oval_btn_bottom.png"
        }
    }

    HueSaturation {
        anchors.fill: buttonBorderImage
        source: buttonBorderImage
        hue: main.hue()
        saturation: main.saturation()
        lightness: main.lightness()
    }

    Rectangle {
        id: background
        color: Constants.transparentColor
        anchors.fill: parent
        anchors.rightMargin: 31
        anchors.leftMargin: 31
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        visible: false
    }

    HueSaturation {
        anchors.fill: background
        source: background
        hue: main.hue()
        saturation: main.saturation()
        lightness: main.lightness()
    }

    MouseArea {
        id: mousearea
        anchors.rightMargin: 15
        anchors.leftMargin: 15
        anchors.bottomMargin: 11
        anchors.topMargin: 10
        anchors.fill: parent
        enabled: !parent.disabled
        onPressed: {
            parent.state = "pressed";
            parent.pressed();
        }
        onReleased: {
            parent.state = "";
            parent.released();
        }
        onClicked: {
            parent.clicked();
        }
        onPressAndHold: {
            parent.pressAndHold();
        }
        onCanceled: {
            parent.state = "";
        }
    }

    Item {
        id: field
        anchors.fill: background

        Icon {
            id: image
            width: Constants.iconButtonSize
            height: Constants.iconButtonSize
            anchors.verticalCenter: parent.verticalCenter
            visible: source ? true : false
        }
        Text {
            id: label
            width: image.visible ? parent.width - image.width : parent.width
            color: Constants.primaryColor
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: Constants.ovalButtonFontSize
            visible: false
            elide: Text.ElideRight
        }
        HueSaturation {
            anchors.fill: label
            source: label
            hue: main.hue()
            saturation: main.saturation()
            lightness: main.lightness()
            visible: label.text !== ""
        }
    }

    states: [
        State {
            name: "pressed"
            PropertyChanges {
                target: left
                source: "../res/buttons/oval_btn_pressed_left.png"
            }

            PropertyChanges {
                target: right
                source: "../res/buttons/oval_btn_pressed_right.png"
            }

            PropertyChanges {
                target: background
                color: Constants.primaryColor
            }

            PropertyChanges {
                target: label
                color: Constants.secondaryColor
            }
        }
    ]
}
