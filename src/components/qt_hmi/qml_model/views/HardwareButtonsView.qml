/**
 * @file HardwareButtonsView.qml
 * @brief Area of screen responsible for hardware buttons emulation
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
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import com.ford.sdl.hmi.hw_buttons 1.0
import "../controls"
import "../hmi_api/Common.js" as Common
import "../views"

Item {
    id: hardwareButtons
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#003"
    }

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

    Column {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 50
        Row {
            Item {
                width: childrenRect.width
                height: childrenRect.height

                HardwareButton { buttonId: Common.ButtonName.TUNEUP; name: "Up" }
                HardwareButton { buttonId: Common.ButtonName.TUNEDOWN; name: "Down" }
                HardwareButton { buttonId: Common.ButtonName.SEEKLEFT; name: "Left" }
                HardwareButton { buttonId: Common.ButtonName.SEEKRIGHT; name: "Right" }
                HardwareButton { buttonId: Common.ButtonName.OK; name: "Ok" }
            }

            Column {
                Row {
                    spacing: 10
                    MaskedButton {
                        name: "vr"
                        onReleased: {
                            if (!dataContainer.activeVR) {
                                vrPopUp.activate();
                            } else {
                                vrPopUp.deactivate();
                            }
                        }
                    }
                    PowerSwitchButton {}
                }

                Grid {
                    columns: 5
                    rows: 2
                    spacing: 5
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
                                color: "white"
                                anchors.centerIn: parent
                            }

                            Timer {
                                id: timer
                                interval: 400
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

        ListModel {
            id: languagesList

            Component.onCompleted: {
                for (var name in Common.Language) {
                    append({name: name.replace('_', '-')});
                }
            }
        }

        Row
        {
            Column
            {
                Text {
                    text: "UI Languages"
                    color: "white"
                }

                ComboBox {
                    model: languagesList
                    width: 200
                }
            }
            Item {
                width: 20
                height: 1
            }

            Column
            {
                Text {
                    text: "TTS + VR Languages"
                    color: "white"
                }

                ComboBox {
                    model: languagesList
                    width: 180
                    onCurrentIndexChanged: {
                        sdlTTS.onLanguageChange(currentIndex);
                        sdlVR.onLanguageChange(currentIndex);
                    }
                }
            }
        }

        Item {
            height: 20
            width: 1
        }

        Text {
            width: 200
            text: "HELP_PROPMT:"
            color: "white"
        }
        Item {
            height: 20
            width: 1
        }

        Text {
            width: 200
            text: "TIMEOUT_PROPMT:"
            color: "white"
        }

        Item {
            height: 20
            width: 1
        }

        Row
        {
            Column {
                PushButton {
                    id: vehicleInfo
                    label: "Vehicle info"
                    toggleMode: true
                    onPressed: {
                        VIPopUp.show()
                    }

                }


                Item {
                    height: 1
                    width: 20
                }

                PushButton {
                    label: "Send data"
                }

                Component {
                    id: tbtStateDelegate
                    TextButton {
                        label: name
                        width: parent.width
                        onClicked: {
                            sdlNavigation.onTBTClientState(value);
                            console.log("Emit signal Navigation.onTBTClientState");
                        }
                    }
                }
                PushButton {
                    id: tbtClientState
                    label: "TBT Client state"
                    toggleMode: true
                    onPressed: {
                        for (var name in Common.TBTState) {
                            selectList.model.append({name: name, value: Common.TBTState[name]});
                        }
                        selectList.delegate = tbtStateDelegate;
                        scrollbar.visible = true;
                    }
                    onUnpressed: {
                        selectList.model.clear();
                        selectList.delegate = null;
                        scrollbar.visible = false;
                    }
                }

                Item {
                    height: 1
                    width: 20
                }

                PushButton {
                    label: "Exit application"
                    onClicked: {
                        if (dataContainer.applicationContext) {
                            sdlBasicCommunication.onExitApplication(dataContainer.currentApplication.appId)
                        }
                    }
                }
                PushButton {
                    id: exitAllAppsButton
                    label: "Exit all apps"
                    toggleMode: true
                    onPressed: {
                        exitAllApplicationsPopup.show()
                    }
                    onUnpressed: {
                        exitAllApplicationsPopup.hide()
                    }
                    Connections {
                        target: exitAllApplicationsPopup
                        onVisibleChanged: {
                            if (!exitAllApplicationsPopup.visible) {
                                exitAllAppsButton.state = "unpressed"
                            }
                        }
                    }
                }
                Row {
                    CheckBox {
                        style: CheckBoxStyle {
                            label: Text {
                                color: "white"
                                text: "Use URL"
                            }
                        }
                    }

                    Item {
                        height: 1
                        width: 20
                    }

                    CheckBox {
                        style: CheckBoxStyle {
                            label: Text {
                                color: "white"
                                text: "DD"
                            }
                        }
                    }
                }
            }

            Column {
                ListView {
                    id: selectList
                    width: 300
                    height: 200

                    model: ListModel {}
                    delegate: TextButton {}

                    Rectangle {
                        id: scrollbar
                        visible: false
                        anchors.right: selectList.right
                        y: selectList.visibleArea.yPosition * selectList.height
                        width: 10
                        height: selectList.visibleArea.heightRatio * selectList.height
                        color: "white"
                    }
                }
            }
        }
    }
}
