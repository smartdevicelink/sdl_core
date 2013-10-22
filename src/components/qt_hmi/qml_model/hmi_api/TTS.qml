import QtQuick 2.0

Item {
    signal onLanguageChange (int language)

    function isReady () {
        return {
            available: dataContainer.hmiTTSAvailable
        }
    }

    function speak (ttsChunks, appID) {
    }

    function stopSpeaking () {
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
//        dataContainer.changeRegistrationTTSVR(language)
    }

    function setGlobalProperties (helpPrompt, timeoutPrompt, appID) {
        dataContainer.globalProperties.helpPrompt = helpPrompt
        dataContainer.globalProperties.timeoutPrompt = timeoutPrompt
    }
}
