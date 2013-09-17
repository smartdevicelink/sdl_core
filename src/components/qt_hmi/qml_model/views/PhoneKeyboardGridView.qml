import QtQuick 2.0
import "../controls"

Column {
    id: phoneKeyboard
    anchors.fill: parent

    width: phoneKeyboard.buttonWidth * 11
    height: phoneKeyboard.buttonHeight * 3
    Row {
        id: upperRow
        property string contentLoader : "ABCDEFGHI"
        anchors.horizontalCenter: parent.horizontalCenter
        Repeater{
            model: 9
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: upperRow.contentLoader.charAt(index)
                pixelSize: 25
                onIsPressedChanged: {
                    if(isPressed == false){
                        contentLoader.go("./views/ContactsListView.qml")
                        contentLoader.item.contactsListView.setCurrent(text)
                    }
                }
            }
        }
    }
    Row {
        id: middleRow
        property string contentLoader : "JKLMNOPQR"
        anchors.horizontalCenter: parent.horizontalCenter
        Repeater{
            model: 9
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: middleRow.contentLoader.charAt(index)
                pixelSize: 25
                onIsPressedChanged: {
                    if(isPressed == false){
                        contentLoader.go("ContactsListView.qml")
                        contentLoader.item.contactsListView.setCurrent(text)
                    }
                }
            }
        }
    }
    Row {
        id: lowerRow
        property string contentLoader : "STUVWXYZ"
        anchors.horizontalCenter: parent.horizontalCenter
        Repeater{
            model: 8
            CircleBtn{
                imgOff: "../res/buttons/preset_btn.png"
                imgOn: "../res/buttons/preset_pressed_btn.png"
                text: lowerRow.contentLoader.charAt(index)
                pixelSize: 25
                onIsPressedChanged: {
                    if(isPressed == false){
                        contentLoader.go("ContactsListView.qml")
                        contentLoader.item.contactsListView.setCurrent(text)
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
                    contentLoader.go("DigitalKeyboardView.qml")
                    contentLoader.item.contactsListView.setCurrent(text)
                }
            }
        }
    }
    BackBtn{}

}




