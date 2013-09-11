import QtQuick 2.0
import com.ford.hmi_framework 1.0

Row {
    signal buttonDown(string name)
    signal buttonUp(string name)

    function pressButton(name) {
        buttonDown(name)
    }

    function releaseButton(name) {
        buttonUp(name)
    }

    MaskedContainer {
        ButtonImage {
            name: "Up"
            source: "res/controlButtons/UpButton.png"
            pressed: "res/controlButtons/UpButton_pressed.png"
        }
        ButtonImage {
            name: "Down"
            source: "res/controlButtons/DownButton.png"
            pressed: "res/controlButtons/DownButton_pressed.png"
        }
        ButtonImage {
            name: "Left"
            source: "res/controlButtons/LeftButton.png"
            pressed: "res/controlButtons/LeftButton_pressed.png"
        }
        ButtonImage {
            name: "Right"
            source: "res/controlButtons/RightButton.png"
            pressed: "res/controlButtons/RightButton_pressed.png"
        }

        ButtonImage {
            name: "Ok"
            source: "res/controlButtons/OkButton.png"
            pressed: "res/controlButtons/OkButton_pressed.png"
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

    Column {
        MaskedContainer {
            ButtonImage {
                id: vrButton
                source: "res/controlButtons/vr.png"
                pressed: "res/controlButtons/vr_pressed.png"
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
