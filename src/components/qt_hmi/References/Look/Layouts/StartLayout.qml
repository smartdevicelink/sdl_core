import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: chrome
    width: 800
    height: 480

    //property alias bg: _bg
    //property alias btnIndicatorState: button1.indicatorState
    property alias button1: button1




    BaseText{
        id: title
        anchors.centerIn: chrome
        text: "Chrome"
    }

    TouchButton{
        id: button1
        anchors.top: title.bottom
        anchors.horizontalCenter: title.horizontalCenter
        text: "Button 1"
    }

}
