import QtQuick 2.0
import com.ford.hmiframework 1.0

FLayout {
    width: 800
    height: 480

    property alias offTouch: offTouch

    Rectangle {
        id: bk
        anchors.fill: parent
        color: "black"
    }

    FWdgtTouchArea{
        id: offTouch
        anchors.fill: bk
    }

/*    RectangularGlow {
        id: effect
        anchors.fill: rect
        glowRadius: 4
        spread: 0.7
        color: "#1d81d5"
        cornerRadius: rect.radius //+ glowRadius
    }

    Rectangle {
        id: rect
        color: "black"
        x:40
        y:40
        width: 160
        height: 45
        radius: 23
        border.width: 1
        border.color: "#1d81d5"
    }

    FWdgtImage{
        id: img
        x:40
        y:90
        bitmap: "OButton.png"
    }

    FWdgtImage{
        id: img
        x:40
        y:40
        bitmap: "Radio/ButtonEnd_8_8.png"
    }

    FWdgtImage{
        id: img1
        anchors.left: img.right
        anchors.top: img.top
        width:2
        fillMode: Image.Stretch
        bitmap: "Radio/ButtonCenter_0_8.png"
    }

    FWdgtImage{
        id: img2
        anchors.left: img1.right
        anchors.top: img1.top
        bitmap: "Radio/ButtonOtherEnd_8_8.png"
    }*/
}
