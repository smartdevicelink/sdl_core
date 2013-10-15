import QtQuick 2.0

Item {
    function isReady () {
        return {
            available: dataContainer.hmiVRAvailable
        }
    }

    function addCommand (cmdID, vrCommands, appID) {
        for (var i = 0; i < vrCommands.length; ++i) {
            dataContainer.vrCommands.push({
                                              cmdID: cmdID,
                                              command: vrCommands[i],
                                              appID: appID
                                          })
        }
    }

    function deleteCommand (cmdID, appID) {
        for (var i = 0; i < dataContainer.vrCommands.length; ) {
            if (dataContainer.vrCommands[i].cmdID === cmdID) {
                if ((appID === undefined) || (dataContainer.vrCommands[i].appID === appID)) {
                    dataContainer.vrCommands.splice(i, 1)
                }
                else {
                    ++i
                }
            }
            else {
                ++i
            }
        }
    }

    function getLanguage () {
        return {
            language: dataContainer.hmiTTSVRLanguage
        }
    }

    function getSupportedLanguages () {
        return {
            languages: settingsContainer.sdlLanguagesList
        }
    }

    function changeRegistration (language) {
        dataContainer.changeRegistrationTTSVR(language)
    }
}
