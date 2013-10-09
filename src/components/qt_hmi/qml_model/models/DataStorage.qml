import QtQuick 2.0
import "../hmi_api/Common.js" as Common

Item {
    ListModel {
        id: deviceListModel
    }

    ListModel {
        id: applicationListModel
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

    property var playerPreferences: {
        "AM": { },
        "FM": { },
        "Sirius": { preset: 0 },
        "BT": { },
        "CD": { },
        "iPod": { }
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
