import QtQuick 2.0
import "./controls"

GridView {
    id: navigationKeyboard

    width: 780
    height: 298
    anchors.fill: parent

        Row {
            x:(navigationKeyboard.width - 63*10) / 2
            y:(navigationKeyboard.height - 63*3) / 2
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "Q"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "W"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "E"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "R"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "T"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "Y"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "U"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "I"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "O"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "P"
                pixelSize: 25
            }
        }

        Row {
            x:(navigationKeyboard.width - 63*11) / 2
            y:(navigationKeyboard.height - 63*3) / 2 + 63

            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "123"
                pixelSize: 20
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "A"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "S"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "D"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "F"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "G"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "H"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "J"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "K"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "L"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                onIsPressedChanged: {
                    hoverImg.source = isPressed ? "res/nav/delete_icon_black.png" : "res/nav/delete_icon.png";
                }
                Image{
                    id: hoverImg
                    anchors.centerIn: parent
                    source: "res/nav/delete_icon.png"
                }
            }
        }

        Row {
            x:(navigationKeyboard.width - 63*10) / 2
            y:(navigationKeyboard.height - 63*3) / 2 + 126
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "!@#"
                pixelSize: 15
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "Z"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "X"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "C"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "V"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "B"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "N"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "M"
                pixelSize: 25
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "Clear"
                pixelSize: 15
            }
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: "Space"
                pixelSize: 15
            }
        }

        Image {
            anchors.right: parent.right
            anchors.top: parent.top
            Text{
                anchors.centerIn: parent
                text: "Go"
                color: "#1d81d5"
                font.pixelSize: 25
            }
            id: goBtn
            source: "res/buttons/short_oval_btn.png"

            MouseArea {
                cursorShape: Qt.PointingHandCursor
                anchors.fill: parent
                onClicked: {
                    menuContainer.go("NavigationInRouteGridView.qml")
                }
            }
        }

        Text{
            id: closeText
            anchors.horizontalCenter: parent.horizontalCenter
            y:250
            text: "CLOSE"
            color: "#1d81d5"
            font.pixelSize: 25
            MouseArea {
              anchors.fill: parent
              onClicked: {
                  menuContainer.go("NavigationMenuGridView.qml")
                }
            }
        }

}




