import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: tileLayout
    width: 800
    height: 480

    property alias button1: button1
    property alias button2: button2
    property alias button3: button3
    property alias button4: button4
    property alias button5: button5
    property alias button6: button6
    property alias button7: button7
    property alias button8: button8
    property alias backBtn: backBtn



    Grid {
        id: theGrid
        flow: Grid.TopToBottom
        anchors.top: tileLayout.top
        anchors.topMargin: 130
        anchors.horizontalCenter: tileLayout.horizontalCenter
        anchors.leftMargin: 100
        columns: 4
        spacing: 20
        TouchButton{
            id: button1
            height:125
            width: 135
            text: "Button 1"
        }

        TouchButton{
            id: button2
            height:125
            width: 135
            text: "Button 2"
        }

        TouchButton{
            id: button3
            height:125
            width: 135
            text: "Button 3"
        }

        TouchButton{
            id: button4
            height:125
            width: 135
            text: "Button 4"
        }

        TouchButton{
            id: button5
            height:125
            width: 135
            text: "Button 5"
        }

        TouchButton{
            id: button6
            height:125
            width: 135
            text: "Button 6"
        }

        TouchButton{
            id: button7
            height:125
            width: 135
            text: "Button 7"
        }

        TouchButton{
            id: button8
            height:125
            width: 135
            text: "Button 8"
        }
    }



    BaseText{
        id: title
        anchors.top: tileLayout.top
        anchors.topMargin: 80
        anchors.horizontalCenter: tileLayout.horizontalCenter
        text: "Select a Source"
    }

    TouchButton{
        id: backBtn
        anchors.verticalCenter: title.verticalCenter
        anchors.left: tileLayout.left
        anchors.leftMargin: 20
        height:40
        width: 60
        text: "<"
    }
}
