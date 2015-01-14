/**
 * @file HardwareButtonsView.qml
 * @brief Area of screen responsible for hardware buttons
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

Rectangle {
    height: row.height + row.anchors.margins
    anchors.left: parent.left
    anchors.right: parent.right
    color: Constants.panelColor

    signal buttonDown(string name)
    signal buttonUp(string name)

    function pressButton(name) {
        buttonDown(name)
    }

    function longPressButton(name) {
        console.log("long press " + name)
    }

    function releaseButton(name) {
        buttonUp(name)
    }

    Row {
        id: row
        spacing: Constants.panelPadding
        anchors.centerIn: parent
        anchors.margins: Constants.panelPadding

        PowerSwitchButton {anchors.verticalCenter: parent.verticalCenter}

        MaskedButton {
            name: "vr"
            anchors.verticalCenter: parent.verticalCenter
            onReleased: {
                console.debug("Clicked VR button");
                if (!dataContainer.activeVR) {
                    vrPopUp.activate();
                    vrHelpPopup.show();
                } else {
                    vrPopUp.complete();
                    vrHelpPopup.hide()
                }
            }
        }

        ArrowKeys {anchors.verticalCenter: parent.verticalCenter}

        Grid {
            columns: 5
            rows: 2
            spacing: 5
            anchors.verticalCenter: parent.verticalCenter
            Repeater {
                model: 10
                delegate : Rectangle {
                    width: 40
                    height: 40
                    radius: 5
                    gradient: Gradient {
                        GradientStop
                        {
                            position: 0.0;
                            color: "#2c2c2c"
                            Behavior on position {
                                NumberAnimation { duration: 80 }
                            }
                        }

                        GradientStop
                        {
                            position: 1.0;
                            color: "black"
                            Behavior on position {
                                NumberAnimation { duration: 80 }
                            }
                        }
                    }

                    Text {
                        text: (1 + index) % 10
                        font.pixelSize: 30
                        color: Constants.panelTextColor
                        anchors.centerIn: parent
                    }

                    Timer {
                        id: timer
                        interval: Constants.presetButtonTimer
                        repeat: false
                        triggeredOnStart: false
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        property bool clickProcessed
                        onPressed: {
                            parent.gradient.stops[0].position = 1.0
                            parent.gradient.stops[1].position = 0.0
                            clickProcessed  = false
                            timer.start()
                            sdlButtons.onButtonEvent(Common.ButtonName.PRESET_0 + index, Common.ButtonEventMode.BUTTONDOWN, undefined)
                        }
                        onReleased: {
                            parent.gradient.stops[0].position = 0.0
                            parent.gradient.stops[1].position = 1.0
                            sdlButtons.onButtonEvent(Common.ButtonName.PRESET_0 + index, Common.ButtonEventMode.BUTTONUP, undefined)
                            timer.stop()
                            if (!clickProcessed) {
                                sdlButtons.onButtonPress(Common.ButtonName.PRESET_0 + index, Common.ButtonPressMode.SHORT, undefined)
                            }
                        }
                        Connections {
                            target: timer
                            onTriggered: {
                                if(!mouseArea.clickProcessed) {
                                    sdlButtons.onButtonPress(Common.ButtonName.PRESET_0 + index, Common.ButtonPressMode.LONG, undefined)
                                    mouseArea.clickProcessed = true
                                }
                            }
                        }
                    }

                    Component.onCompleted: {
                        settingsContainer.buttonCapabilities.push(
                        {
                            name: Common.ButtonName.PRESET_0 + index,
                            upDownAvailable: true,
                            shortPressAvailable: true,
                            longPressAvailable: true
                        });
                    }
                }
            }
        }
    }
}
