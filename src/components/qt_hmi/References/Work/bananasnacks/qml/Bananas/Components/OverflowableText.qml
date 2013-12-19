import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    id: oflow_text
    property alias text: text.text
    property alias font: text.font
    property alias fontColor: text.color
    property string textAlign
    color: Qt.rgba(0,0,0,0)
    height: text.font.pixelSize * 1.2
    clip: true
    Text {
        id: text
        Component.onCompleted: {
            if (oflow_text.width < width) {
                anchors.left = parent.left
            }
            else if (oflow_text.textAlign == 'center') {
                anchors.horizontalCenter = parent.horizontalCenter
            }
        }

        NumberAnimation on anchors.leftMargin {
            id: scroll_animation
            to: oflow_text.width - text.width
            duration: ((oflow_text.width - text.width) < 0) ? (text.width - oflow_text.width) * 15 : 0
            running: false
            onStopped: {
                restart_timer.start()
            }
        }
    }

    Timer {
        id: scroll_timer
        running: true
        interval: 3000
        onTriggered: { if (oflow_text.width < text.width) scroll_animation.start();}
    }
    Timer {
        id: restart_timer
        interval: 3000
        onTriggered: text.anchors.leftMargin = 0
    }
}
