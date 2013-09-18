import QtQuick 2.0
import "../controls"

Item {
    id: navKeyboard
    anchors.fill: parent

    Item {
        // 3/4 top screen
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        // Text line with input text
        TextInput{
            id: inputText
            anchors.top:parent.top
            anchors.left:parent.left
            maximumLength: 30
            color: "#1d81d5"
            font.pixelSize: 30
        }

        // Keyboard
        Column {
            anchors.centerIn: parent
            Row {
                id: upperRow
                property string qWERTY : "QWERTYUIOP"
                anchors.horizontalCenter: parent.horizontalCenter

                Repeater{
                    model: 10
                    CircleBtn{
                        imgOff: "../res/buttons/preset_btn.png"
                        imgOn: "../res/buttons/preset_pressed_btn.png"
                        text: upperRow.qWERTY.charAt(index)
                        pixelSize: 25

                        onClicked: {
                            inputText.text += upperRow.qWERTY.charAt(index)
                        }
                    }
                }
            }
            Row {
                id: middleRow
                property string aSDFGH : "ASDFGHJKL"
                anchors.horizontalCenter: parent.horizontalCenter

                CircleBtn{
                    imgOff: "../res/buttons/preset_btn.png"
                    imgOn: "../res/buttons/preset_pressed_btn.png"
                    text: "123"
                    pixelSize: 15
                }
                //--------------
                Repeater{
                    model: 9
                    CircleBtn{
                        imgOff: "../res/buttons/preset_btn.png"
                        imgOn: "../res/buttons/preset_pressed_btn.png"
                        text: middleRow.aSDFGH.charAt(index)
                        pixelSize: 25

                        onClicked: {
                            inputText.text += middleRow.aSDFGH.charAt(index)
                        }
                    }
                }
                //--------------
                CircleBtn{
                    imgOff: "../res/buttons/preset_btn.png"
                    imgOn: "../res/buttons/preset_pressed_btn.png"
                    onIsPressedChanged: {
                        hoverImg.source = isPressed ? "../res/nav/delete_icon_black.png" : "../res/nav/delete_icon.png";
                    }
                    Image{
                        id: hoverImg
                        anchors.centerIn: parent
                        source: "../res/nav/delete_icon.png"
                    }

                    onClicked: {
                        inputText.text = inputText.text.substring(0,inputText.text.length - 1)
                    }
                }
            }
            Row{
                id: lowerRow
                property string zXCVBN : "ZXCVBNM"
                anchors.horizontalCenter: parent.horizontalCenter

                CircleBtn{
                    imgOff: "../res/buttons/preset_btn.png"
                    imgOn: "../res/buttons/preset_pressed_btn.png"
                    text: "!@#"
                    pixelSize: 15
                }
                //-----------------
                Repeater{
                    model: 7
                    CircleBtn{
                        imgOff: "../res/buttons/preset_btn.png"
                        imgOn: "../res/buttons/preset_pressed_btn.png"
                        text: lowerRow.zXCVBN.charAt(index)
                        pixelSize: 25

                        onClicked: {
                            inputText.text += lowerRow.zXCVBN.charAt(index)
                        }
                    }
                }
                //----------------
                CircleBtn{
                    imgOff: "../res/buttons/preset_btn.png"
                    imgOn: "../res/buttons/preset_pressed_btn.png"
                    text: "Clear"
                    pixelSize: 12

                    onClicked: {
                        inputText.text = ""
                    }
                }
                //----------------
                CircleBtn{
                    imgOff: "../res/buttons/preset_btn.png"
                    imgOn: "../res/buttons/preset_pressed_btn.png"
                    text: "Space"
                    pixelSize: 12

                    onClicked: {
                        inputText.text += " "
                    }
                }
            }

        } // column

       Image {
            id: goBtn
            anchors.right: parent.right
            anchors.top: parent.top
            source: "../res/buttons/short_oval_btn.png"

            Text{
                id: goText
                anchors.centerIn: parent
                text: "Go"
                color: "#1d81d5"
                font.pixelSize: 25
            }

            MouseArea {
                anchors.fill: parent

                onPressed: {
                    goBtn.source = "../res/buttons/short_oval_btn_pressed.png"
                    goText.color = "black"
                }

                onReleased: {
                    goBtn.source = "../res/buttons/short_oval_btn.png"
                    goText.color = "#1d81d5"
                }

                onClicked: {
                    contentLoader.go("./views/NavigationInRouteGridView.qml")
                }
            }
        }
    } // 3/4 top screen

    Item{
        // 1/4 bottom screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        BackBtn {
            onIsPressedChanged: {
                if(isPressed == false){
                    contentLoader.go("./views/NavigationMenuGridView.qml")
                }
            }
        }
    }
}
