import QtQuick 2.0

Rectangle {
    width: 1360
    height: 768
    color: "black"

    Text {
        id: caption
        color: "lightblue"
        text: "Hello"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    GridView
    {
        width: 600
        height: 400
        x : (parent.width - width) / 2
        y : (parent.height - height) / 2

        cellWidth: 140
        cellHeight: 140

        model: Menu {}
        // highlight: Rectangle { color: "lightsteelblue"; radius: 5; width: parent.width; height: parent.height + 20 }
        focus: false

        delegate: Column {
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    caption.text = name
                }
            }
            x: 20
            y: 20
            width: 100
            height: 100
            Image { source: icon; anchors.horizontalCenter: parent.horizontalCenter }
            Text  {
                text: name;
                color: "white"
                anchors.baseline: parent.bottom;
                anchors.horizontalCenter: parent.horizontalCenter }
            }

    }
//*/

    /*
    Item
    {
        id: hardwareKeybezel1
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
    //*/
}

