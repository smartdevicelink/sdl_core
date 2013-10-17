/**
 * @file DataStorage.qml
 * @brief Container for data.
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
import "../hmi_api/Common.js" as Common

Item {

    property string contactsFirstLetter // first letter of contact's name that need to find at contact list
    property ApplicationModel currentApplication: ApplicationModel { }

    function getApplication(appId) {
        for(var i = 0; i < applicationList.count; i++) {
            if(applicationList.get(i).appId === appId) {
                return applicationList.get(i);
            }
        }
    }

    property int systemContext

    function setCurrentApplication(appId) {
        var oldApplicationContext = applicationContext;
        applicationContext = false;

        for(var i = 0; i < applicationList.count; i++) {
            if(applicationList.get(i).appId === appId) {
                currentApplication.appId = appId
                currentApplication.appName = applicationList.get(i).appName
                currentApplication.appType = applicationList.get(i).appType
                // ... etc
            }
        }

        applicationContext = oldApplicationContext;

        currentApplicationChanged()
    }

    function addApplication(app) {
        applicationList.append(
        {
            appName: app.appName,
            ngnMediaScreenAppName: app.ngnMediaScreenAppName,
            icon: app.icon,
            deviceName: app.deviceName,
            appId: app.appId,
            hmiDisplayLanguageDesired: app.hmiDisplayLanguageDesired,
            isMediaApplication: app.isMediaApplication,
            appType: app.appType,
            helpPrompt: "",
            timeoutPrompt: ""
        })
    }

    function removeApplication(appId) {
        for (var i = 0; i < applicationList.count; i++) {
            if (applicationList.get(i).appId === appId) {
                applicationList.remove(i);
                break;
            }
        }
    }

    property bool applicationContext: false

    property int systemSavedContext
    property bool applicationSavedContext

    property string route_text: ""

    property PlayerState cdPlayerState: PlayerState {
        playPauseState: 'Pause'
        albumImage: "../res/album_art.png"
        trackNumber: "13/16"
        trackName: "The Dog Days Are Over"
        albumName: "Florence and the Machine"
    }
    property PlayerState ipodPlayerState: PlayerState {
        albumImage: "../res/album_art.png"
        trackNumber: "13/16"
        trackName: "The Dog Days Are Over"
        albumName: "Florence and the Machine"
        playPauseState: 'Pause'
    }
    property PlayerState usbPlayerState: PlayerState {
        playPauseState: 'Pause'
        albumImage: "../res/album_art.png"
        trackNumber: "13/16"
        trackName: "The Dog Days Are Over"
        albumName: "Florence and the Machine"
    }
    property PlayerState linePlayerState: PlayerState {
        playPauseState: 'Pause'
        albumImage: "../res/album_art.png"
        trackNumber: "13/16"
        trackName: "The Dog Days Are Over"
        albumName: "Florence and the Machine"
    }
    property PlayerState sdlPlayerState: PlayerState {
        playPauseState: 'Pause'
        albumImage: "../res/album_art.png"
        trackNumber: "13/16"
        trackName: "The Dog Days Are Over"
        albumName: "Florence and the Machine"
    }
    property PlayerState btPlayerState: PlayerState {
        playPauseState: 'Pause'
        albumImage: "../res/album_art.png"
        trackNumber: "13/16"
        trackName: "The Dog Days Are Over"
        albumName: "Florence and the Machine"
    }
    property PlayerState amPlayerState: PlayerState {
        songName: "So Far Around The Bend"
        albumName: "The National"
        presets: [ "1130", "950", "760", "1270", "1400", "2100", "940", "1001" ]
    }
    property PlayerState fmPlayerState: PlayerState {
        songName: "So Far Around The Bend"
        albumName: "The National"
        presets: ["96.3", "107.9", "104.3", "101.9", "105.3", "100.5", "107.9", "103.4"]
    }
    property PlayerState siriusPlayerState: PlayerState {
        songName: "So Far Around The Bend"
        albumName: "The National"
        presets: [ "Lithium", "Spectrum", "ESPN", "Alt Nation", "Lithium", "Spectrum", "ESPN", "Alt Nation" ]
    }

    property bool hmiVRAvailable: false
    property bool hmiTTSAvailable: false
    property bool hmiNavigationAvailable: false
    property bool hmiVehicleInfoAvailable: false
    property bool hmiUIAvailable: false

    property int hmiUILanguage: Common.Language.EN_US
    property int hmiTTSVRLanguage: Common.Language.EN_US

    property var hmiUIText: {
        "mainField1": "The Dog Days Are Over",
        "mainField2": "Florence and the Machine",
        "mainField3": "Track 13/16",
        "mainField4": "",
        "statusBar": "",
        "mediaClock": "02:36"
    }
    property int hmiUITextAlignment: Text.AlignLeft

    property ListModel deviceList: ListModel {
        ListElement {
            name: "Device 1"
            devid: 1
        }
    }
    property ListModel applicationList: ListModel {
        ListElement {
            appName: "SyncProxyTester"
            icon: "../res/SPT.png"
            deviceName: "Device 1"
            appId: 1
            hmiDisplayLanguageDesired: 0
            isMediaApplication: 1
        }
    }

    property var vrCommands: []

    property var globalProperties: {
        "helpPrompt": "",
        "timeoutPrompt": ""
    }

    property int uiSliderPosition: 1

    function reset () {
        route_text = ""
    }

    function changeRegistrationUI (language) {
        hmiUILanguage = language
    }

    function changeRegistrationTTSVR (language) {
        hmiTTSVRLanguage = language
    }

    property NavigationModel navigationModel: NavigationModel { }
    property bool activeVR: false
}
