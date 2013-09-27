import QtQuick 2.0
import QtQuick.Window 2.0

Window {
    width: 400
    height: 256

    title: "Alert"
    flags: Qt.Dialog

    Rectangle {
        id: rectangle
        anchors.fill: parent

        property alias alertString: text.text

        Text {
            id: text
        }
    }

    Timer {
        id: timer
        onTriggered: hide()
    }

    function alert (alertString, duration) {
        rectangle.alertString = alertString
        timer.interval = duration
        timer.start()
        show()
    }
}
