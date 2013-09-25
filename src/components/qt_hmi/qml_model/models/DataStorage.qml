import QtQuick 2.0
import "../hmi_api/Common.js" as Common

Item {
    ListModel {
        id: deviceListModel
        ListElement {
            name: "Device 1"
            devid: 1
        }
        ListElement {
            name: "Device 2"
            devid: 2
        }
        ListElement {
            name: "Device 3"
            devid: 3
        }
    }

    ListModel {
        id: applicationListModel
        ListElement {
            appName: "SyncProxyTester"
            icon: "../res/SPT.png"
        }
    }


    property string route_text: ""

    property var playerPreferences: {
        "AM": { },
        "FM": { },
        "Sirius": { preset: 0 },
        "BT": { },
        "CD": { },
        "iPod": { }
    }

    property int hmiUILanguage: Common.Language.EN_US
    property int hmiTTSVRLanguage: Common.Language.EN_US

    property alias deviceList: deviceListModel
    property alias applicationList: applicationListModel

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
