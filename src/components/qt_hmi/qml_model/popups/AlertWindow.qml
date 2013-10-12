import QtQuick 2.0
import QtQuick.Window 2.0

Window {
    width: 400
    height: 256

    title: "Alert"
    flags: Qt.Dialog

    property var async

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
        onTriggered: { hide(); DBus.sendReply(async, {} ) } // TODO(nvaganov@luxoft.com): check for other alerts and return "try again"
    }

    function alert (alertString, duration) {
        rectangle.alertString = alertString
        timer.interval = duration
        timer.start()
        show()
    }
}
