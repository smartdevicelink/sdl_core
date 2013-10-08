import QtQuick 2.0
import "../controls"

GeneralView{
    Item {
        id: phoneKeyboard
        anchors.fill: parent

        Item {
            // 3/4 top screen
            height: parent.height * 3/4
            width: parent.width
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            Column {
                anchors.centerIn: parent

                Row {
                    id: upperRow
                    property string contentLoader : "ABCDEFGHI"
                    anchors.horizontalCenter: parent.horizontalCenter
                    Repeater {
                        model: 9
                        CircleBtn {
                            imgOff: "../res/buttons/preset_btn.png"
                            imgOn: "../res/buttons/preset_pressed_btn.png"
                            text: upperRow.contentLoader.charAt(index)
                            pixelSize: 25
                            onIsPressedChanged: {
                                if(isPressed == false){
                                    dataContainer.contactsFirstLetter = text
                                    contentLoader.go("./views/ContactsListView.qml")
                                }
                            }
                        }
                    }
                }

                Row {
                    id: middleRow
                    property string contentLoader : "JKLMNOPQR"
                    anchors.horizontalCenter: parent.horizontalCenter
                    Repeater {
                        model: 9
                        CircleBtn {
                            imgOff: "../res/buttons/preset_btn.png"
                            imgOn: "../res/buttons/preset_pressed_btn.png"
                            text: middleRow.contentLoader.charAt(index)
                            pixelSize: 25
                            onIsPressedChanged: {
                                if(isPressed == false){
                                    dataContainer.contactsFirstLetter = text
                                    contentLoader.go("./views/ContactsListView.qml")
                                }
                            }
                        }
                    }
                }

                Row {
                    id: lowerRow
                    property string contentLoader : "STUVWXYZ"
                    anchors.horizontalCenter: parent.horizontalCenter
                    Repeater {
                        model: 8
                        CircleBtn {
                            imgOff: "../res/buttons/preset_btn.png"
                            imgOn: "../res/buttons/preset_pressed_btn.png"
                            text: lowerRow.contentLoader.charAt(index)
                            pixelSize: 25
                            onIsPressedChanged: {
                                if(isPressed == false){
                                    dataContainer.contactsFirstLetter = text
                                    contentLoader.go("./views/ContactsListView.qml")
                                }
                            }
                        }
                    }
                    CircleBtn {
                        imgOff: "../res/buttons/preset_btn.png"
                        imgOn: "../res/buttons/preset_pressed_btn.png"
                        text: "123"
                        pixelSize: 20
                    }
                }
            }
        }

        Item {
            // 1/4 bottom screen
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width
            height: 1/4 * parent.height

            BackButton { anchors.horizontalCenter: parent.horizontalCenter }
        }
    }
}


