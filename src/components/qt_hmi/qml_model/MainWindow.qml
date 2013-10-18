/**
 * @file MainWindow.qml
 * @brief Implementation of main window.
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
import QtMultimedia 5.0
import com.ford.sdl.hmi.dbus_adapter 1.0
import com.ford.sdl.hmi.hw_buttons 1.0
import "./controls"
import "./views"
import "./hmi_api" as HmiApi
import "./models"
import "./popups"
import "hmi_api/Common.js" as Common
import "./models/Constants.js" as Constants

Rectangle {
    width: 1600
    height: 768
    property string startQml: "./views/AMPlayerView.qml"
    property int minWidth: Constants.mainScreenMinWidth
    property int minHeight: Constants.mainScreenMiHeight
    color: Constants.secondaryColor

    DataStorage {
        id: dataContainer

        onSystemContextChanged: {
            sdlUI.onSystemContext(systemContext)
        }

        // Please note that applicationContext is changed only after setting currentApplication
        onApplicationContextChanged: {
            if (applicationContext) {
                sdlBasicCommunication.onAppActivated(currentApplication.appId)
            } else {
                sdlBasicCommunication.onAppDeactivated(currentApplication.appId, contentLoader.item.category)
            }
        }
    }

    Audio {
        id: beep
        source: "res/initial.wav"
    }

    SettingsStorage {
        id: settingsContainer
    }

    Item {
        id: mainScreen
        x: 0
        y: 0
        anchors.top: parent. top
        anchors.left: parent.left
        width: (parent.width * 0.62 < minWidth) ? minWidth : (parent.width * 0.62)
        // TODO {ALeshin}: Screen width shouldn't be static, remove 62% width and 38% width
        height: (parent.height < minHeight) ? minHeight : parent.height
        visible: false

        Item {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.25
            width: parent.width
            HeaderMenu {}
        }

        Item {
            anchors.leftMargin: Constants.margin
            anchors.rightMargin: Constants.margin
            anchors.bottomMargin: Constants.margin
            anchors.fill: parent

            Loader {
                id: contentLoader
                asynchronous: true
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height * 0.75
                width: parent.width
                source:startQml
                property var viewTransitionStack : []

                function reset() {
                    viewTransitionStack = []
                }

                property string position
                function go(path, appId) {
                    if (position !== path) {
                        viewTransitionStack.push(source.toString())
                        if (appId) {
                            dataContainer.setCurrentApplication(appId)
                        }
                        position = path
                        source = path
                    }
                }

                function back() {
                    source = viewTransitionStack.pop()
                }

                onStatusChanged: {
                    if (status == Component.Ready) {
                        if (dataContainer.systemContext !== Common.SystemContext.SYSCTXT_ALERT) {
                            dataContainer.systemContext = item.systemContext
                            dataContainer.applicationContext = item.applicationContext
                        }
                        else {
                            dataContainer.systemSavedContext = item.systemContext
                            dataContainer.applicationSavedContext = item.applicationContext
                        }
                    }
                }
            }
        }

        AlertWindow {
            id: alertWindow
            objectName: "AlertWindow"
            anchors.fill: parent
            visible: false
        }

        VRPopUp {
            id: vrPopUp
            anchors.fill: parent
            visible: false
        }

        ExitAllApplicationsPopup {
            id: exitAllApplicationsPopup
            anchors.fill: parent
            visible: false
        }
    }

    Item {
        id: hwBtnScreen
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: mainScreen.right
        width: parent.width * 0.38
        height: (parent.height < minHeight) ? minHeight : parent.height
        HardwareButtonsView {}
    }

    HMIProxy {
        HmiApi.ButtonsProxy {
            id: sdlButtons
            objectName: "Buttons"
        }
        HmiApi.BasicCommunicationProxy {
            id: sdlBasicCommunication
            objectName: "BasicCommunication"
        }
        HmiApi.VRProxy {
            id: sdlVR
            objectName: "VR"
        }
        HmiApi.TTSProxy {
            id: sdlTTS
            objectName: "TTS"
        }
        HmiApi.NavigationProxy {
            id: sdlNavigation
            objectName: "Navigation"
        }
        HmiApi.VehicleInfoProxy {
            id: sdlVehicleInfo
            objectName: "VehicleInfo"
        }
        HmiApi.UIProxy {
            id: sdlUI
            objectName: "UI"
        }
    }

    SDLProxy {
        id: sdlProxy

        onAppRegistered: {
            dataContainer.addApplication(
            {
                 appName: application.appName,
                 ngnMediaScreenAppName: application.ngnMediaScreenAppName,
                 icon: application.icon,
                 deviceName: application.deviceName,
                 appId: application.appId,
                 hmiDisplayLanguageDesired: application.hmiDisplayLanguageDesired,
                 isMediaApplication: application.isMediaApplication,
                 appType: application.appType
             });
        }

        onAppUnregistered: {
            dataContainer.removeApplication(appId);
        }

        onPlayTone: {
            beep.play()
        }
    }

    Component.onCompleted: {
        dataContainer.hmiVRAvailable = true
        dataContainer.hmiTTSAvailable = true
        dataContainer.hmiNavigationAvailable = true
        dataContainer.hmiVehicleInfoAvailable = true
        dataContainer.hmiUIAvailable = true

        sdlBasicCommunication.onReady()
    }
}
