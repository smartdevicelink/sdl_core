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
    property ApplicationData hmiUIText: ApplicationData {
    }

    property string helpPrompt
    property string timeoutPrompt

    property ListModel options: ListModel {
    }
}
