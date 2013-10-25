import QtQuick 2.0
import "Common.js" as Common

Item {
    signal onLanguageChange (int language)

    function isReady () {
        return {
            available: dataContainer.hmiTTSAvailable
        }
    }

    function speak (ttsChunks, appID) {
    }

    function stopSpeaking() {
        console.debug("Stop speaking");
        ttsPopUp.deactivate();
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

    function getCapabilities() {
        return {
            capabilities: Common.SpeechCapabilities.SC_TEXT
        }
    }

    function changeRegistration(language, appID) {
        console.debug("ChangeRegistrationTTSVR:", language, appID);
        dataContainer.changeRegistrationTTSVR(language, appID)
    }

    function setGlobalProperties (helpPrompt, timeoutPrompt, appID) {
        dataContainer.globalProperties.helpPrompt = helpPrompt
        dataContainer.globalProperties.timeoutPrompt = timeoutPrompt
    }
}
