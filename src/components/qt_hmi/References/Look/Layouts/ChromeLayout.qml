import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: chromelayout
    width: 800
    height: 480

    property alias button1: button1
    property alias button2: button2
    property alias button3: button3



    FWdgtTouchArea {
        id: blockClockTouch
        anchors.top: chromelayout.top
        anchors.horizontalCenter: chromelayout.horizontalCenter
        height: 60
        width: 160
    }

    Rectangle{
        id: clockBackground
        anchors.fill: blockClockTouch
        color: "red"
    }

    BaseText{
        id: clock
        anchors.centerIn: blockClockTouch
        text: "clock"
        horAlignment: Text.AlignHCenter
    }

    TouchButton{
        id: button1
        anchors.top: blockClockTouch.top
        anchors.right: blockClockTouch.left
        anchors.rightMargin: -5
        height: blockClockTouch.height
        width: 75
        text: "Button 1"
        unpressedColor: "darkRed"
    }

    TouchButton{
        id: button2
        anchors.top: blockClockTouch.top
        anchors.left: blockClockTouch.right
        anchors.leftMargin: -5
        height:blockClockTouch.height
        width: 75
        text: "Button 2"
        unpressedColor:"darkRed"
    }

    TouchButton{
        id: button3
        anchors.bottom: chromelayout.bottom
        anchors.horizontalCenter: chromelayout.horizontalCenter
        height:blockClockTouch.height
        width: 225
        text: "Button 3"
        unpressedColor: "red"
    }

}
