import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"

FLayout {
    id: home
    width: 800
    height: 480

    property alias button1: button1
    property alias button2: button2
    property alias button3: button3
    property alias button4: button4




    TouchButton{
        id: button1
        anchors.top: home.top
        anchors.left: home.left
        height:235
        width: 395
        text: "Button 1"
    }

    TouchButton{
        id: button2
        anchors.top: home.top
        anchors.right: home.right
        height:235
        width: 395
        text: "Button 2"
    }

    TouchButton{
        id: button3
        anchors.bottom: home.bottom
        anchors.left: home.left
        height:235
        width: 395
        text: "Button 3"
    }

    TouchButton{
        id: button4
        anchors.bottom: home.bottom
        anchors.right: home.right
        height:235
        width: 395
        text: "Button 4"
    }

    BaseText{
        id: title
        anchors.centerIn: home
        text: "Home"
    }


}
