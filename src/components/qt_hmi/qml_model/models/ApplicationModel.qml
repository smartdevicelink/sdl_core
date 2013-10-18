import QtQuick 2.0

QtObject {
    property int appId
    property string appName
    property string ngnMediaScreenAppName
    property url icon
    property string deviceName
    property int hmiDisplayLanguageDesired
    property bool isMediaApplication
    property var appType

    // media player
    property string playPauseState
    property var hmiUIText: {
        "mainField1": "The Dog Days Are Over",
        "mainField2": "Florence and the Machine",
        "mainField3": "Track 13/16",
        "mainField4": "",
        "statusBar": "",
        "mediaClock": "02:36",
        "picture": "../res/album_art.png"
    }

    property string helpPrompt
    property string timeoutPrompt
    property ListModel options: ListModel {
    }
    property ListModel turnList
    property ListModel turnListSoftButtons
    // This place is for adding new properties
}
