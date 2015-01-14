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

import QtQuick 1.1
import com.ford.sdl.hmi.dbus_adapter 1.0
import com.ford.sdl.hmi.log4cxx 1.0
import "./controls"
import "./views"
import "./hmi_api" as HmiApi
import "./models"
import "./popups"
import "hmi_api/Common.js" as Common
import "./models/Constants.js" as Constants
import "models/Internal.js" as Internal

Rectangle {
    width: 1280
    height: 768
    property string startQml: "./views/ApplicationListView.qml"
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

//    TODO(KKolodiy): find way for play sound
//    Audio {
//        id: beep
//        source: "res/initial.wav"
//    }

    SettingsStorage {
        id: settingsContainer
    }

    Item {
        id: mainScreen
        anchors.top: parent. top
        anchors.left: parent.left
        width: (parent.width - simulationScreen.width < minWidth) ?
                   minWidth : parent.width - simulationScreen.width
        height: (parent.height - hardwareScreen.height < minHeight) ?
                    minHeight : parent.height - hardwareScreen.height
        clip: true
        visible: false

        Item {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.10
            width: parent.width
            HeaderMenu {}
        }

        Item {
            height: parent.height * 0.90
            width: parent.width
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            Loader {
                id: contentLoader
                height: parent.height - Constants.margin
                width: parent.width - 2 * Constants.margin
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.leftMargin: Constants.margin
                anchors.rightMargin: Constants.margin
                anchors.bottomMargin: Constants.margin


                source:startQml
                property variant viewTransitionStack : []

                function reset() {
                    viewTransitionStack = []
                }

                property string currentLocation
                function go(path, appId) {
                    console.debug("enter:", path, ", appId:", appId)
                    if (path) {
                        if (currentLocation !== path) {
                            viewTransitionStack.push({ uri: source.toString(), applicationContext: false })
                            if (appId) {
                                dataContainer.setCurrentApplication(appId)
                            }
                            currentLocation = path
                            source = path
                        }
                    }
                    console.debug("exit")
                }

                function back() {
                    if (viewTransitionStack.length) {
                        source = viewTransitionStack.pop().uri
                    }
                    currentLocation = ""
                }

                onStatusChanged: {
                    if (status == Component.Ready) {
                        if (!dataContainer.activeAlert) {
                            dataContainer.applicationContext = item.applicationContext
                        }
                        else {
                            dataContainer.applicationSavedContext = item.applicationContext
                        }
                        if (viewTransitionStack && viewTransitionStack.length) {
                            viewTransitionStack[viewTransitionStack.length - 1].applicationContext = item.applicationContext
                        }
                        dataContainer.setSystemContext()
                    }
                }
            }
        }

        WarningInfo { id: warningInfo }

        InteractionPopup {
            id: interactionPopup
            popUpName: "UI.PerformInteraction"
            anchors.centerIn: parent
            visible: false
        }

        SliderPopup {
            id: sliderPopup
            popUpName: "UI.Slider"
            anchors.centerIn: parent
            visible: false
        }

        PerformAudioPassThruPopup {
            id: performAudioPassThruPopup
            popUpName: "UI.PerformAudioPassThru"
            anchors.centerIn: parent
            visible: false
        }

        VRHelpPopup {
            id: vrHelpPopup
            popUpName: "UI.VrHelp"
            anchors.centerIn: parent
            visible: false
        }

        AlertWindow {
            id: alertWindow
            popUpName: "UI.Alert"
            objectName: "AlertWindow"
            anchors.fill: parent
            visible: false
        }
    }

    Item {
        id: simulationScreen
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: mainScreen.right
        width: simulationPanel.width
        clip: true
        SimulationView {
            id: simulationPanel

            VRPopUp {
                id: vrPopUp
                visible: false
                anchors.fill: parent
            }

            TTSPopUp {
                id: ttsPopUp
                anchors.top: parent.top
                anchors.right: parent.right
                visible: false
            }
        }
    }

    Item {
        id: hardwareScreen
        anchors.top: mainScreen.bottom
        anchors.left: parent.left
        anchors.right: simulationScreen.left
        height: hardwarePanel.height
        clip: true
        HardwareButtonsView { id: hardwarePanel }
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
                appType: application.appType,
                hmiUIText: { },
                mediaClock: {
                    "updateMode": Internal.MediaClockUpdateMode.MCU_COUNTUP,
                    "runningMode": Internal.MediaClockRunningMode.MCR_STOPPED,
                    "startTime": -1,
                    "endTime": -1,
                    "startTimeForProgress": -1
                }
             });
        }

        onAppUnregistered: {
            console.debug("enter")
            dataContainer.removeApplication(appId);
            if ((dataContainer.currentApplication.appId === appId)) {
                if (dataContainer.applicationContext) {
                    contentLoader.go("views/ApplicationListView.qml");
                }
                if (contentLoader.viewTransitionStack.filter(function(x) { return x.applicationContext })) {
                    contentLoader.reset();
                }
                dataContainer.currentApplication.reset()
            }
            console.debug("exit")
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

    /*
      * Test Support Section
      *
      */
    function getSlider(){
        return sliderPopup
    }

    function getDataStorage(){
        return dataContainer
    }

    function getUIProxy(){
        return sdlUI
    }

    function getMainScreen(){
        return mainScreen
    }

    function getContentLoader(){
        return contentLoader
    }

    function getWarningInfo(){
        return warningInfo
    }
    /*
      * Test Support Section End
      */
}
