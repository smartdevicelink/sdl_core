import QtQuick 2.0
import "../hmi_api/Common.js" as Common

Item {
    ListModel {
        id: deviceListModel
        ListElement {
            name: "Device 1"
            devid: 1
        }
    }

    ListModel {
        id: applicationListModel
        ListElement {
            appName: "SyncProxyTester"
            icon: "../res/SPT.png"
            deviceName: "Device 1"
            appId: 1
            hmiDisplayLanguageDesired: 0
            isMediaApplication: 1
        }
    }

    property int applicationId
    property var currentApplication: applicationContext ?
                                         getApplication(applicationId) :
                                         undefined;

    function getApplication(appId) {
        for(var i = 0; i < applicationList.count; i++) {
            if(applicationList.get(i).appId === appId) {
                return applicationList.get(i);
            }
        }
    }

    property int hmiContext
    property bool applicationContext: false

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



    property var playerPreferences: {
        "AM":     { },
        "FM":     { },
        "Sirius": { preset: 0 },
        "BT":     { },
        "CD":     { },
        "iPod":   { },
        "SDL":    { }
    }

    property bool hmiVRAvailable: false
    property bool hmiTTSAvailable: false
    property bool hmiNavigationAvailable: false
    property bool hmiVehicleInfoAvailable: false
    property bool hmiUIAvailable: false

    property int hmiUILanguage: Common.Language.EN_US
    property int hmiTTSVRLanguage: Common.Language.EN_US

    property alias deviceList: deviceListModel
    property alias applicationList: applicationListModel

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
}
