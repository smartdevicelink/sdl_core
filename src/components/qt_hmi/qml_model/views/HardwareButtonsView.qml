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
                    spacing: 25
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
                    PowerSwitchBtn {}
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
            ListElement { lang: 'EN-US' }
            ListElement { lang: 'ES-MX' }
            ListElement { lang: 'FR-CA' }
            ListElement { lang: 'DE-DE' }
            ListElement { lang: 'ES-ES' }
            ListElement { lang: 'EN-GB' }
            ListElement { lang: 'RU-RU' }
            ListElement { lang: 'TR-TR' }
            ListElement { lang: 'PL-PL' }
            ListElement { lang: 'FR-FR' }
            ListElement { lang: 'IT-IT' }
            ListElement { lang: 'SV-SE' }
            ListElement { lang: 'PT-PT' }
            ListElement { lang: 'NL-NL' }
            ListElement { lang: 'ZH-TW' }
            ListElement { lang: 'JA-JP' }
            ListElement { lang: 'AR-SA' }
            ListElement { lang: 'KO-KR' }
            ListElement { lang: 'PT-BR' }
            ListElement { lang: 'CS-CZ' }
            ListElement { lang: 'DA-DK' }
            ListElement { lang: 'NO-NO' }
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
                ToggleButton {
                    label: "Vehicle info"
                }

                Item {
                    height: 1
                    width: 20
                }

                ToggleButton {
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
                ToggleButton {
                    id: tbtClientState
                    label: "TBT Client state"
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

                ToggleButton {
                    label: "Exit application"
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
