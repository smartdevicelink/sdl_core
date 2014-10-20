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
import "../controls"
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants
import "../models/Internal.js" as Internal

Rectangle {
    id: alertWindow
    property date lastAlertTime
    property var softButtons
    property int appId
    property string popUpName // TODO{ALESHIN}: No requerments for closePopUp, if alert couldn't be closed by closePopUp request - delete everywhere

    color: "transparent"

    property var async

    Rectangle {
        id: rectangle

        anchors.centerIn: parent
        color: "black"
        border.color: "white"

        width: alertContent.width + 60
        height: alertContent.height + 60

        property alias alertString: alert.text
        property alias appNameString: appName.text
        property int duration

        Column {
            id: alertContent
            anchors.centerIn: parent
            spacing: 20

            Text {
                id: appName
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.pointSize: 16
            }

            Row {
                width: Constants.alertWidth
                spacing: 20

                Image {
                    id: alertLogo
                    source: "../res/warning.png"
                }

                Text {
                    id: alert
                    width: parent.width - alertLogo.width - parent.spacing
                    clip: true
                    color: "white"
                    font.pointSize: 16
                }
            }

            Rectangle {
                id: progressIndicator
                color: "white"
                height: 10
            }

            Column {
                Item {
                    width: Constants.alertWidth
                    height: alertButton1.visible ? alertButton1.height : 0

                    SoftButton {
                        id: alertButton1
                        appId: alertWindow.appId
                        button: softButtons && softButtons.length > 0 ? softButtons[0] : undefined
                        anchors.left: parent.left
                        anchors.right: alertButton2.visible ? alertButton2.left : parent.right
                        width: Constants.alertWidth / 2
                        onPressed: { alertWindow.keep = true; }
                        onReleased: { alertWindow.keep = false; }
                        onKeepContext: {
                            alertWindow.restart();
                            sdlUI.onResetTimeout(appId, "UI.Alert")
                        }
                        onDefaultAction: { alertWindow.complete(Common.Result.SUCCESS); }
                        onStealFocus: {
                            alertWindow.complete(Common.Result.SUCCESS);
                            contentLoader.go( Internal.chooseAppStartScreen(dataContainer.currentApplication.appType,
                                                                           dataContainer.currentApplication.isMediaApplication), appId )
                        }
                    }
                    SoftButton {
                        id: alertButton2
                        appId: alertWindow.appId
                        button: softButtons && softButtons.length > 1 ? softButtons[1] : undefined
                        anchors.right: parent.right
                        width: Constants.alertWidth / 2
                        onPressed: { alertWindow.keep = true; }
                        onReleased: { alertWindow.keep = false; }
                        onKeepContext: {
                            alertWindow.restart();
                            sdlUI.onResetTimeout(appId, "UI.Alert")
                        }
                        onDefaultAction: { alertWindow.complete(Common.Result.SUCCESS); }
                        onStealFocus: {
                            alertWindow.complete(Common.Result.SUCCESS);
                            contentLoader.go( Internal.chooseAppStartScreen(dataContainer.currentApplication.appType,
                                                                           dataContainer.currentApplication.isMediaApplication), appId )
                        }
                    }
                }
                Item {
                    width: Constants.alertWidth
                    height: alertButton3.visible ? childrenRect.height : 0
                    SoftButton {
                        id: alertButton3
                        appId: alertWindow.appId
                        button: softButtons && softButtons.length > 2 ? softButtons[2] : undefined
                        anchors.left: parent.left
                        anchors.right: alertButton4.visible ? alertButton4.left : parent.right
                        width: Constants.alertWidth / 2
                        onPressed: { alertWindow.keep = true; }
                        onReleased: { alertWindow.keep = false; }
                        onKeepContext: {
                            alertWindow.restart();
                            sdlUI.onResetTimeout(appId, "UI.Alert")
                        }
                        onDefaultAction: { alertWindow.complete(Common.Result.SUCCESS); }
                        onStealFocus: {
                            alertWindow.complete(Common.Result.SUCCESS);
                            contentLoader.go( Internal.chooseAppStartScreen(dataContainer.currentApplication.appType,
                                                                           dataContainer.currentApplication.isMediaApplication), appId )
                        }
                    }
                    SoftButton {
                        id: alertButton4
                        appId: alertWindow.appId
                        button: softButtons && softButtons.length > 3 ? softButtons[3] : undefined
                        anchors.right: parent.right
                        width: Constants.alertWidth / 2
                        onPressed: { alertWindow.keep = true; }
                        onReleased: { alertWindow.keep = false; }
                        onKeepContext: {
                            alertWindow.restart();
                            sdlUI.onResetTimeout(appId, "UI.Alert")
                        }
                        onDefaultAction: { alertWindow.complete(Common.Result.SUCCESS); }
                        onStealFocus: {
                            alertWindow.complete(Common.Result.SUCCESS);
                            contentLoader.go( Internal.chooseAppStartScreen(dataContainer.currentApplication.appType,
                                                                           dataContainer.currentApplication.isMediaApplication), appId )
                        }
                    }
                }
            }
        }

        SequentialAnimation {
            id: animation
            PropertyAction { target: progressIndicator; property: "width"; value: alertContent.width }
            PropertyAnimation {
                id: shrinkProgressAnimation
                target: progressIndicator
                property: "width"
                to: 0
                duration: rectangle.duration
            }
        }
    }

    Timer {
        id: timer
        onTriggered: {
            complete(Common.Result.SUCCESS)
        }
    }

    function alert (alertStrings, duration, sButtons, showIndicator, alertType, applicationId) {
        if (timer.running) { // we have alert already
            var currentTime = new Date()
            var timeFromLastAlert = currentTime - lastAlertTime
            var timeLeft = timer.interval - timeFromLastAlert
            var discreteInMilliseconds = 1000 // wish to round left time to integer seconds
            var timeLeftRounded = discreteInMilliseconds * Math.ceil(timeLeft / discreteInMilliseconds)
            return timeLeftRounded
        }
        else {
            lastAlertTime = new Date();
            appId = applicationId
            rectangle.appNameString = dataContainer.getApplication(appId).appName;
            softButtons = sButtons;
            rectangle.alertString = alertStrings.join('\n');
            timer.interval = duration;
            rectangle.duration = duration;
            timer.start();
            show();

            progressIndicator.visible = !!showIndicator
            progressIndicator.width = alertContent.width
            console.log("ProgressIndicator.width:", progressIndicator.width)
            animation.start()
        }
    }

    function show () {
        dataContainer.activeAlert = true
        dataContainer.applicationSavedContext = dataContainer.applicationContext
        visible = true
    }

    function complete (reason, data) {
        if (!keep) {
            hide()
            switch (reason) {
            case Common.Result.SUCCESS:
                DBus.sendReply(async, { __retCode: Common.Result.SUCCESS, __message: "UI.Alert" })
                break
            // For other cases
            }
        }
        timer.stop()
    }

    function hide() {
        console.debug(popUpName, "HIDE")
        dataContainer.activeAlert = false
        dataContainer.applicationContext = dataContainer.applicationSavedContext
        visible = false
    }

    function restart() {
        animation.restart();
        timer.restart();
    }

    property bool keep: false

    onKeepChanged: {
        if (visible && !keep && !timer.running) {
            hide()
        }
    }

    onVisibleChanged: {
        if (visible) {
            dataContainer.activePopup.push(popUpName)
        } else {
            for (var i in dataContainer.activePopup) {
                if (dataContainer.activePopup[i] === popUpName) {
                    dataContainer.activePopup.splice(i, 1)
                }
            }
        }
    }
}
