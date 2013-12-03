// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

Item {
    id: scrollbar
    property variant target

    clip: true

    //anchors {top: target.top; bottom: target.bottom; right: target.right }
    visible: (track.height == slider.height) ? false : true //TODO: !visible -> width: 0 (but creates a binding loop)

    Timer {
        property int scrollAmount

        id: timer
        repeat: true
        interval: 20
        onTriggered: {
            target.contentY = Math.max(
                0, Math.min(
                    target.contentY + scrollAmount,
                    target.contentHeight - target.height));
        }
    }

    MouseArea {
        anchors.fill: scrollbar
        onPressed: {
            timer.scrollAmount = target.height * (mouseY < slider.y ? -1 : 1)       // scroll by a page
            timer.running = true;
        }
        onReleased: {
            timer.running = false;
        }
    }

    Rectangle {
        id: track

        color: "red"
        opacity: 0.3
        radius: 2
        smooth: true

        anchors.top: scrollbar.top
        anchors.bottom: scrollbar.bottom
        anchors.horizontalCenter: scrollbar.horizontalCenter
        width: 3
    }

    Rectangle {
        id:slider

        width: scrollbar.width
        color: "red"
        opacity: 0.7
        radius: 4
        smooth: true

        height: Math.min(target.height / target.contentHeight * track.height, track.height)
        y: target.visibleArea.yPosition * track.height

        MouseArea {
            anchors.fill: parent
            drag.target: parent
            drag.axis: Drag.YAxis
            drag.minimumY: 0
            drag.maximumY: track.height - height

            onPositionChanged: {
                if (pressedButtons == Qt.LeftButton) {
                    target.contentY = slider.y * target.contentHeight / track.height
                }
            }
        }
    }
}
