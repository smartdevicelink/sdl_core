import QtQuick 2.0
import QtQuick.Window 2.0
import "../hmi_api/Common.js" as Common

Rectangle {
    color: "transparent"

    MouseArea {
        anchors.fill: parent
    }

    Rectangle {
        id: rectangle

        anchors.centerIn: parent
        width: 400
        height: 256
        color: "black"
        border.color: "white"

        property alias alertString: alert.text
        property alias appNameString: appName.text

        MouseArea {
            anchors.fill: parent
            onClicked: hide()
        }

        Column {

            anchors.fill: parent
            anchors.topMargin: 20
            anchors.leftMargin: 20
            spacing: 20

            Text {
                id: appName
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.pointSize: 16
                horizontalAlignment: Text.AlignHCenter
            }

            Row {

                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 20

                Image {
                    source: "../res/warning.png"
                }

                Text {
                    id: alert
                    width: 200
                    clip: true
                    color: "white"
                    font.pointSize: 16
                }

            }
        }
    }

    Timer {
        id: timer
        onTriggered: hide()
    }

    property date lastAlertTime

    function alert (alertString, duration, appID) {
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
            rectangle.appNameString = ""
            for (var appIndex = 0; appIndex < dataContainer.applicationList.count; ++appIndex) {
                if (dataContainer.applicationList.get(appIndex).appId === appID) {
                    rectangle.appNameString = dataContainer.applicationList.get(appIndex).appName
                    break
                }
            }
            rectangle.alertString = alertString
            timer.interval = duration
            timer.start()
            show()
        }
    }

    function show () {
        dataContainer.hmiSavedContext = dataContainer.hmiContext
        dataContainer.hmiContext = Common.SystemContext.SYSCTXT_ALERT
        dataContainer.applicationSavedContext = dataContainer.applicationContext
        visible = true
    }

    function hide () {
        dataContainer.hmiContext = dataContainer.hmiSavedContext
        dataContainer.applicationContext = dataContainer.applicationSavedContext
        visible = false
    }
}
