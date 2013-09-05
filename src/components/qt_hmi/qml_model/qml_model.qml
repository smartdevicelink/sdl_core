import QtQuick 2.0
import com.ford.hmiframework 1.0

Rectangle {
    width: 1360
    height: 768
    color: "black"

    Text {
        anchors.centerIn: parent
        text: "Hello World"
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    Item
    {
        id: FHardwareKeybezel1
        width: 800
        height: 375
        x:10
        y:500

        // function keys
        TextHardkey {
            id: hk_OnOff
            width: 100
            height: 50
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            label: "On/Off"
        }

        BaseText {
            id: info
            text: dataPool.version
            color: "black"
            anchors.top: hk_OnOff.top
            anchors.left: hk_OnOff.right
            anchors.leftMargin: 20
        }


        FHardwareKey {
                   id: buttonUp
                   hardwareControlID: "buttonUp"
                   //commName: "buttonBack"

                   x:0
                   y:0
                   width: 60
                   height:25


                   keyboardKey: Qt.Key_Up
                   visible: false
               }

        FHardwareKey {
                   id: buttonDown
                   hardwareControlID: "buttonDown"
                   //commName: "buttonBack"

                   x:0
                   y:0
                   width: 60
                   height:25


                   keyboardKey: Qt.Key_Down
                   visible: false
               }

    }
}

