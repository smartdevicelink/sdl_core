// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

BorderImage {
        property variant target

        source: "../Resources/scrollbar.png"
        border {left: 0; top: 3; right: 0; bottom: 3}
        width: 17

        //anchors {top: target.top; bottom: target.bottom; right: target.right }
        visible: (track.height == slider.height) ? false : true //TODO: !visible -> width: 0 (but creates a binding loop)

        Item {
                anchors {fill: parent; margins: 1; rightMargin: 2; bottomMargin: 2}

                Image {
                        id: upArrow
                        source: "../Resources/up-arrow.png"
                        anchors.top: parent.top
                        MouseArea {
                                anchors.fill: parent
                                onPressed: {
                                        timer.scrollAmount = -10
                                        timer.running = true;
                                }
                                onReleased: {
                                        timer.running = false;
                                }
                        }
                }

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

                Item {
                        id: track
                        anchors {top: upArrow.bottom; topMargin: 1; bottom: dnArrow.top;}
                        width: parent.width

                        MouseArea {
                                anchors.fill: parent
                                onPressed: {
                                        timer.scrollAmount = target.height * (mouseY < slider.y ? -1 : 1)       // scroll by a page
                                        timer.running = true;
                                }
                                onReleased: {
                                        timer.running = false;
                                }
                        }

                        BorderImage {
                                id:slider

                                source: "../Resources/slider.png"
                                border {left: 0; top: 3; right: 0; bottom: 3}
                                width: parent.width

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
                Image {
                        id: dnArrow
                        source: "../Resources/dn-arrow.png"
                        anchors.bottom: parent.bottom
                        MouseArea {
                                anchors.fill: parent
                                onPressed: {
                                        timer.scrollAmount = 10
                                        timer.running = true;
                                }
                                onReleased: {
                                        timer.running = false;
                                }
                        }
                }
        }
}
