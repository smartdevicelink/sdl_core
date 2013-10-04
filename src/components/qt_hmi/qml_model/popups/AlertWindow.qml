import QtQuick 2.0
import QtQuick.Window 2.0

Rectangle {
    color: "transparent"

    MouseArea {
        anchors.fill: parent
    }

    Rectangle {
        id: rectangle

        width: 400
        height: 256

        property alias alertString: text.text

        Text {
            id: text
        }
    }

    Timer {
        id: timer
        onTriggered: hide()
    }

    property date lastAlertTime

    function alert (alertString, duration) {
        if (timer.running) { // we have alert already
            var currentTime = new Date()
            var timeFromLastAlert = currentTime - lastAlertTime
            var timeLeft = timer.interval - timeFromLastAlert
            var discreteInMilliseconds = 1000 // wish to round left time to integer seconds
            var timeLeftRounded = discreteInMilliseconds * Math.ceil(timeLeft / discreteInMilliseconds)
            return timeLeftRounded
        }
        else {
            lastAlertTime = new Date()
            rectangle.alertString = alertString
            timer.interval = duration
            timer.start()
            visible = true
        }
    }

    function hide () {
        visible = false
    }
}
