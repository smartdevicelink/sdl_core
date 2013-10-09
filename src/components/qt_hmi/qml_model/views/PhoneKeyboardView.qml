import QtQuick 2.0
import "../controls"
import "../models"

GeneralView{
    Component.onCompleted: phoneKeyboard.changeColorOfActiveButtons()

    Item {
        property string defaultColor: "#1d81d5"
        property string emptyLetterColor: "grey"
        property var activeButtons: [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0] // 26 letters in the alphabet

        ContactsListModel {
            id: contactsListModel
        }

        function changeColorOfActiveButtons() {
            for (var i = 0; i < contactsListModel.count; i++) {
                var Aindex = "A".charCodeAt(0);
                var index = contactsListModel.get(i).name.charCodeAt(0) - Aindex;
                activeButtons[index] = 1 // if contact started with current letter exists - change color
            }
        }

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
                            textColorDefault: phoneKeyboard.activeButtons[upperRow.contentLoader.charCodeAt(index) - "A".charCodeAt(0)]
                                       ? phoneKeyboard.defaultColor : phoneKeyboard.emptyLetterColor
                            text: upperRow.contentLoader.charAt(index)
                            pixelSize: 25
                            onIsPressedChanged: {
                                if (isPressed == false){
                                    if (phoneKeyboard.activeButtons[upperRow.contentLoader.charCodeAt(index) - "A".charCodeAt(0)]) {
                                        dataContainer.contactsFirstLetter = text
                                        contentLoader.go("./views/ContactsListView.qml")
                                    }
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
                            textColorDefault: phoneKeyboard.activeButtons[middleRow.contentLoader.charCodeAt(index) - "A".charCodeAt(0)]
                                       ? phoneKeyboard.defaultColor : phoneKeyboard.emptyLetterColor
                            text: middleRow.contentLoader.charAt(index)
                            pixelSize: 25
                            onIsPressedChanged: {
                                if (isPressed == false){
                                    if (phoneKeyboard.activeButtons[middleRow.contentLoader.charCodeAt(index) - "A".charCodeAt(0)]) {
                                        dataContainer.contactsFirstLetter = text
                                        contentLoader.go("./views/ContactsListView.qml")
                                    }
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
                            textColorDefault: phoneKeyboard.activeButtons[lowerRow.contentLoader.charCodeAt(index) - "A".charCodeAt(0)]
                                       ? phoneKeyboard.defaultColor : phoneKeyboard.emptyLetterColor
                            text: lowerRow.contentLoader.charAt(index)
                            pixelSize: 25
                            onIsPressedChanged: {
                                if (isPressed == false){
                                    if (phoneKeyboard.activeButtons[lowerRow.contentLoader.charCodeAt(index) - "A".charCodeAt(0)]) {
                                        dataContainer.contactsFirstLetter = text
                                        contentLoader.go("./views/ContactsListView.qml")
                                    }
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

            BackButton { anchors.centerIn: parent}
        }
    }
}


