import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import com.ford.hmi_framework 1.0
import "../controls"

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

    function releaseButton(name) {
        buttonUp(name)
    }

    Column {
        anchors.centerIn: parent
        Row {
            MaskedContainer {
                ButtonImage {
                    name: "Up"
                    source: "../res/controlButtons/UpButton.png"
                    pressed: "../res/controlButtons/UpButton_pressed.png"
                }
                ButtonImage {
                    name: "Down"
                    source: "../res/controlButtons/DownButton.png"
                    pressed: "../res/controlButtons/DownButton_pressed.png"
                }
                ButtonImage {
                    name: "Left"
                    source: "../res/controlButtons/LeftButton.png"
                    pressed: "../res/controlButtons/LeftButton_pressed.png"
                }
                ButtonImage {
                    name: "Right"
                    source: "../res/controlButtons/RightButton.png"
                    pressed: "../res/controlButtons/RightButton_pressed.png"
                }

                ButtonImage {
                    name: "Ok"
                    source: "../res/controlButtons/OkButton.png"
                    pressed: "../res/controlButtons/OkButton_pressed.png"
                }

                property ButtonImage pressed: null;

                onPressed: {
                    attr.item.pressedOpacity = 1
                    pressed = attr.item;
                    hardwareButtons.pressButton(attr.item.name)
                }
                onReleased: {
                    if (pressed != null) {
                        pressed.pressedOpacity = 0
                        hardwareButtons.releaseButton(pressed.name)
                        pressed = null
                    }
                }
            }

            Item {
                width: 20
                height: 1
            }

            Column {
                Row {
                    spacing: 25
                    MaskedContainer {
                        ButtonImage {
                            id: vrButton
                            source: "../res/controlButtons/vr.png"
                            pressed: "../res/controlButtons/vr_pressed.png"
                        }
                        onPressed: {
                            vrButton.pressedOpacity = 1
                            hardwareButtons.pressButton("Vr")
                        }
                        onReleased: {
                            vrButton.pressedOpacity = 0
                            hardwareButtons.releaseButton("Vr")
                        }
                    }
                    PowerSwitchBtn{}
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
                            property bool pressed;
                            gradient: Gradient {
                                GradientStop { position: pressed ? 1.0 : 0.0; color: "#2c2c2c" }
                                GradientStop { position: pressed ? 0.0 : 1.0; color: "black" }
                            }

                            Text {
                                text: (1 + index) % 10
                                font.pixelSize: 30
                                color: "white"
                                anchors.centerIn: parent
                            }
                            MouseArea {
                                anchors.fill: parent
                                onPressed: {
                                    parent.pressed = true
                                }
                                onReleased: {
                                    parent.pressed = false
                                }
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
            Rectangle {
                width: 160
                height: 40

                property bool pressed;

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#2c2c2c" }
                    GradientStop { position: 1.0; color: "black" }
                }

                Text {
                    text: "Vehicle info"
                    font.pixelSize: 18
                    color: "white"
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        parent.gradient.stops[0].position = 1.0
                        parent.gradient.stops[1].position = 0.0
                    }
                    onReleased: {
                        parent.gradient.stops[0].position = 0.0
                        parent.gradient.stops[1].position = 1.0
                    }
                }
            }

            Item {
                height: 1
                width: 20
            }

            Rectangle {
                width: 160
                height: 40

                property bool pressed;

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#2c2c2c" }
                    GradientStop { position: 1.0; color: "black" }
                }

                Text {
                    text: "Send data"
                    font.pixelSize: 18
                    color: "white"
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        parent.gradient.stops[0].position = 1.0
                        parent.gradient.stops[1].position = 0.0
                    }
                    onReleased: {
                        parent.gradient.stops[0].position = 0.0
                        parent.gradient.stops[1].position = 1.0
                    }
                }
            }

        }

        Row
        {
            Rectangle {
                width: 160
                height: 40

                property bool pressed;

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#2c2c2c" }
                    GradientStop { position: 1.0; color: "black" }
                }

                Text {
                    text: "TBT Client state"
                    font.pixelSize: 18
                    color: "white"
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        parent.gradient.stops[0].position = 1.0
                        parent.gradient.stops[1].position = 0.0
                    }
                    onReleased: {
                        parent.gradient.stops[0].position = 0.0
                        parent.gradient.stops[1].position = 1.0
                    }
                }
            }

            Item {
                height: 1
                width: 20
            }

            Rectangle {
                width: 160
                height: 40

                property bool pressed;

                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#2c2c2c" }
                    GradientStop { position: 1.0; color: "black" }
                }

                Text {
                    text: "Exit application"
                    font.pixelSize: 18
                    color: "white"
                    anchors.centerIn: parent
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        parent.gradient.stops[0].position = 1.0
                        parent.gradient.stops[1].position = 0.0
                    }
                    onReleased: {
                        parent.gradient.stops[0].position = 0.0
                        parent.gradient.stops[1].position = 1.0
                    }
                }
            }
        }

        Item {
            height: 20
            width: 1
        }

        Row
        {
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

        /*Button {
            width: 100
            height: 40
            text: "Vehicle info"
            //font.pixelSize: 18
            //color: "white"
            anchors.centerIn: parent
        }*/
    }
}
