import QtQuick 2.0

Item {
    property bool available: false

    function isReady () {
        return {
            available: available
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
}
