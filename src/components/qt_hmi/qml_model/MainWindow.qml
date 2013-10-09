/**
 * @file MainWindow.qml
 * @brief Main QML model
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
import com.ford.sdl.hmi.dbus_adapter 1.0
import com.ford.sdl.hmi.hw_buttons 1.0
import "./controls"
import "./views"
import "./hmi_api" as HmiApi
import "./models"
import "./popups"
import "hmi_api/Common.js" as Common

Rectangle {
    width: 1600
    height: 768
    property string startQml: "./views/AMPlayerView.qml"
    property int margin: 20
    property int minWidth: 800
    property int minHeight: 600
    color: "black"

    DataStorage {
        id: dataContainer

        onHmiContextChanged: {
            sdlUI.onSystemContext(hmiContext)
        }

        onApplicationContextChanged: {
            if (applicationContext) {
                sdlBasicCommunication.onAppActivated(applicationId)
            } else {
                sdlBasicCommunication.onAppDeactivated(applicationId, contentLoader.item.category)
            }
        }
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
        height: (parent.height < minHeight) ? minHeight : parent.height
        anchors.leftMargin: 0
        anchors.topMargin: 0
        visible: false

        Item {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.25
            width: parent.width
            HeaderMenu {}
        }

        Item {
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.bottomMargin: 30
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

                function reset(){
                    viewTransitionStack = []
                    source = startQml
                }

                function go(path) {
                    viewTransitionStack.push(source.toString())
                    source = path
                }

                function back() {
                    source = viewTransitionStack.pop()
                }

                onStatusChanged: {
                    if (status == Component.Ready) {
                        if (dataContainer.hmiContext !== Common.SystemContext.SYSCTXT_ALERT) {
                            dataContainer.hmiContext = item.systemContext
                            dataContainer.applicationContext = item.applicationContext
                        }
                        else {
                            dataContainer.hmiSavedContext = item.systemContext
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
            dataContainer.applicationList.append(
            {
                 appName: application.appName,
                 ngnMediaScreenAppName: application.ngnMediaScreenAppName,
                 icon: application.icon,
                 deviceName: application.deviceName,
                 appId: application.appId,
                 hmiDisplayLanguageDesired: application.hmiDisplayLanguageDesired,
                 isMediaApplication: application.isMediaApplication,
                 appType: application.appType
             })
        }

        onAppUnregistered: {
            for (var i = 0; i < dataContainer.applicationList.count; i++) {
                if (dataContainer.applicationList.get(i).appId === appId) {
                    dataContainer.applicationList.remove(0);
                    break;
                }
            }
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
