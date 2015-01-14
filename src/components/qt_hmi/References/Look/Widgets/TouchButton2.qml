import QtQuick 2.0
import com.ford.hmiframework 1.0
import QtGraphicalEffects 1.0
import "../Widgets"



Item {
    width: 160
    height: 45

    RectangularGlow {
        id: effect
        anchors.fill: rect
        glowRadius: 10
        spread: 0.2
        color: "#1d81d5"
        cornerRadius: rect.radius + glowRadius
    }

    Rectangle {
        id: rect
        color: "black"
        anchors.centerIn: parent
        width: 160
        height: 45
        radius: 25
        border.color: "#1d81d5"
    }
}


