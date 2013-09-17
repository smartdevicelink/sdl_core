import QtQuick 2.0
import "./controls"

Column {
    id: phoneKeyboard
    anchors.fill: parent

    width: phoneKeyboard.buttonWidth * 11
    height: phoneKeyboard.buttonHeight * 3
    Row {
        id: upperRow
        property string content : "ABCDEFGHI"
        anchors.horizontalCenter: parent.horizontalCenter
        Repeater{
            model: 9
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: upperRow.content.charAt(index)
                pixelSize: 25
                onIsPressedChanged: {
                    if(isPressed == false){
                        menuContainer.go("ContactsListView.qml")
                        menuContainer.item.contactsListView.setCurrent(text)
                    }
                }
            }
        }
    }
    Row {
        id: middleRow
        property string content : "JKLMNOPQR"
        anchors.horizontalCenter: parent.horizontalCenter
        Repeater{
            model: 9
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: middleRow.content.charAt(index)
                pixelSize: 25
                onIsPressedChanged: {
                    if(isPressed == false){
                        menuContainer.go("ContactsListView.qml")
                        menuContainer.item.contactsListView.setCurrent(text)
                    }
                }
            }
        }
    }
    Row {
        id: lowerRow
        property string content : "STUVWXYZ"
        anchors.horizontalCenter: parent.horizontalCenter
        Repeater{
            model: 8
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: lowerRow.content.charAt(index)
                pixelSize: 25
                onIsPressedChanged: {
                    if(isPressed == false){
                        menuContainer.go("ContactsListView.qml")
                        menuContainer.item.contactsListView.setCurrent(text)
                    }
                }
            }
        }
        CircleBtn{
            imgOff: "../res/buttons/preset_btn.png"
            imgOn: "../res/buttons/preset_pressed_btn.png"
            text: "123"
            pixelSize: 20
            onIsPressedChanged: {
                if(isPressed == false){
                    menuContainer.go("DigitalKeyboardView.qml")
                    menuContainer.item.contactsListView.setCurrent(text)
                }
            }
        }
    }
    BackBtn{}

}




