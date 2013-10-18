/**
 * @file AlertWindow.qml
 * @brief Alert popup window
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

import QtQuick 2.0
import QtQuick.Window 2.0
import "../controls"
import "../hmi_api/Common.js" as Common

Rectangle {
    color: "transparent"

    property var async

    Rectangle {
        id: rectangle

        anchors.centerIn: parent
        color: "black"
        border.color: "white"

        width: alertContent.width + 60
        height: alertContent.height + 60

        MouseArea {
            anchors.fill: alertContent
            onClicked: complete()
        }

        property alias alertString: alert.text
        property alias appNameString: appName.text

        Column {
            id: alertContent
            anchors.centerIn: parent
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
            Column {
                Item {
                    width: 360
                    height: alertButton1.visible ? alertButton1.height : 0
                    OvalButton {
                        id: alertButton1
                        text: softButtons && softButtons.length > 0 ? softButtons[0].text : ""
                        anchors.left: parent.left
                        anchors.right: alertButton2.visible ? alertButton2.left : parent.right
                        width: 180
                        visible: softButtons ? softButtons.length > 0 : false
                    }
                    OvalButton {
                        id: alertButton2
                        text: softButtons && softButtons.length > 1 ? softButtons[1].text : ""
                        width: 180
                        visible: softButtons ? softButtons.length > 1 : false
                        anchors.right: parent.right
                    }
                }
                Item {
                    width: 360
                    height: alertButton3.visible ? childrenRect.height : 0
                    OvalButton {
                        id: alertButton3
                        text: softButtons && softButtons.length > 2 ? softButtons[2].text : ""
                        anchors.right: alertButton4.visible ? alertButton4.left : parent.right
                        width: 180
                        visible: softButtons ? softButtons.length > 2 : false
                        anchors.left: parent.left
                    }
                    OvalButton {
                        id: alertButton4
                        text: softButtons && softButtons.length > 3 ? softButtons[3].text : ""
                        width: 180
                        visible: softButtons ? softButtons.length > 3 : false
                        anchors.right: parent.right
                    }
                }
            }
        }
    }

    Timer {
        id: timer
        onTriggered: {
            complete()
        }
    }

    property date lastAlertTime
    property var softButtons

    function alert (alertStrings, duration, appID, sButtons) {
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
            rectangle.appNameString = dataContainer.getApplication(appID).appName
            var alertString = ""
            for (var index in alertStrings) {
                alertString += alertStrings[index]
                alertString += "\n"
            }
            softButtons = sButtons
            rectangle.alertString = alertString
            timer.interval = duration
            timer.start()
            show()
        }
    }

    function show () {
        dataContainer.systemSavedContext = dataContainer.systemContext
        dataContainer.systemContext = Common.SystemContext.SYSCTXT_ALERT
        dataContainer.applicationSavedContext = dataContainer.applicationContext
        visible = true
    }

    function complete () {
        timer.stop()
        dataContainer.systemContext = dataContainer.systemSavedContext
        dataContainer.applicationContext = dataContainer.applicationSavedContext
        visible = false
        DBus.sendReply(async, {})
    }
}
