/**
 * @file PresetRow.qml
 * @brief Animated row with radio stations.
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

Item {
    id: presetRow
    height: childrenRect.height
    property variant presets: []
    property int selectedIndex: 0
    signal presetSelected    
    signal presetButtonPressed()
    signal presetButtonReleased()
    signal presetButtonClicked()
    signal presetButtonHold()

    Image {
        id: circleButton
        source: "../res/buttons/preset_pressed_btn.png"
        visible: false
        enabled: false
    }

    PagedFlickable {
        width: parent.width
        spacing: (width - (circleButton.width * 4)) / 3
        snapTo: spacing + circleButton.width
        elementWidth: circleButton.width

        Repeater {
            model: presetRow.presets.length
            delegate:
                Column {
                    width: circleButton.width
                    Image {
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: presetRow.selectedIndex === index ? "../res/buttons/preset_pressed_btn.png" : "../res/buttons/preset_btn.png"
                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                presetRow.selectedIndex = index;
                                presetButtonPressed()
                            }
                            onReleased: {
                                presetRow.selectedIndex = index;
                                presetButtonReleased()
                            }
                            onClicked: {
                                presetRow.selectedIndex = index
                                presetButtonClicked()
                            }
                            onPressAndHold: {
                                presetRow.selectedIndex = index;
                                presetButtonHold()
                            }
                        }
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            text: index + 1
                            font.pixelSize: Constants.fontSize
                            color: presetRow.selectedIndex === index ? Constants.secondaryColor : Constants.primaryColor
                        }
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: presetRow.presets[index]
                        font.pixelSize: Constants.fontSize
                        color: "white"
                    }
            }
        }
    }
}
