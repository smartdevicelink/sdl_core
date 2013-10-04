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

    property date lastAlertTime

    function alert (alertString, duration) {
        if (timer.running) { // we have alert already
            var currentTime = new Date()
            var timeFromLastAlert = currentTime - lastAlertTime
            var timeLeft = timer.interval - timeFromLastAlert
            var discreteInMilliseconds = 1000 // wish to round left time to integer seconds
            var timeLeftRounded = discreteInMilliseconds * Math.ceil(timeLeft / discreteInMilliseconds)
            return {
                alertLaunched: false,
                timeToWait: timeLeftRounded
            }
        }
        else {
            lastAlertTime = new Date()
            rectangle.alertString = alertString
            timer.interval = duration
            timer.start()
            show()
            return {
                alertLaunched: true
            }
        }
    }
}
