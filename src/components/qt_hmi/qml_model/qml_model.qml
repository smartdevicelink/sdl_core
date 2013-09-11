import QtQuick 2.0
import com.ford.hmiplugin 1.0

Rectangle {
    id: mainScreen
    width: 1480
    height: 768
    color: "black"

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    PowerSwitcher {
        x: parent.x + 50
        anchors.verticalCenter: parent.verticalCenter
        onSwitched: {
            if (sw === "on") {
                mainScreen.state = "Main"
                menuContainer.source = "MainMenuGridView.qml"
            } else {
                mainScreen.state = ""
                menuContainer.source = ""
            }
        }
    }

    Item {
        id: headerMenu
        x: menuContainer.x + menuContainer.width / 2 - 400
        y: 120
        width: 800
        height: 80
        Text{
            text: "75°";
            color: "#1d81d5"
            font.pixelSize: 25;
            width: parent.width / parent.columns
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            Text {
                text: "≡"
                color: "#1d81d5"
                font.pixelSize: 25
                anchors.horizontalCenter: parent.horizontalCenter
                y: parent.y - 55
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    menuContainer.source = "MainMenuGridView.qml"
                }
            }

            text: "MENU";
            color: "#1d81d5"
            font.pixelSize: 25
            width: parent.width / parent.columns
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Text{
            text: "12:50";
            color: "#1d81d5"
            font.pixelSize: 25;
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }

        visible: mainScreen.state !== ""
    }

    property Item mmm;

    Loader {
        id: menuContainer
        x: 120
        y: (parent.height - height) / 2
        width: 884
        height: 298
    }

    Row {
        id: hardwareButtons

        signal buttonDown(string name)
        signal buttonUp(string name)

        function pressButton(name) {
            buttonDown(name)
        }

        function releaseButton(name) {
            buttonUp(name)
        }

        x: menuContainer.x + menuContainer.width + 20
        anchors.verticalCenter: parent.verticalCenter
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

            MaskedContainer {

            }
        }
    }
}

