import QtQuick 2.0
import "../controls"

QtObject {
    property int appId
    property string appName
    property string ngnMediaScreenAppName
    property Icon appIcon: Icon {
    }
    property string deviceName
    property int hmiDisplayLanguageDesired
    property bool isMediaApplication
    property var appType

    // media player
    property string playPauseState
    property ApplicationData hmiUIText: ApplicationData {
    }
    property MediaClockModel mediaClock: MediaClockModel {
    }

    property string helpPrompt
    property string timeoutPrompt
    property ListModel options: ListModel {
    }

    property ListModel currentSubMenu: options
    property ListModel turnList
    property ListModel softButtons
    property ListModel turnListSoftButtons
    property int languageTTSVR
    // This place is for adding new properties

    function reset () {
        console.debug("enter")
        appId = -1
        appName = ""
        ngnMediaScreenAppName = ""
        appIcon = null
        deviceName = ""
        hmiDisplayLanguageDesired = -1
        isMediaApplication = false
        appType = undefined
        playPauseState = ""
        hmiUIText.reset()
// no need to reset mediaClock
        helpPrompt = ""
        timeoutPrompt = ""
        options = null
        currentSubMenu = options
        turnList = null
        softButtons = null
        turnListSoftButtons = null
        languageTTSVR = -1
        console.debug("exit")
    }
}
