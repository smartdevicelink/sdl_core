import QtQuick 2.0

Item {
    property bool available: false

    signal onLanguageChange (int language)

    function isReady () {
        return {
            available: available
        }
    }

    function getLanguage () {
        return {
            language: dataContainer.hmiUILanguage
        }
    }

    function getSupportedLanguages () {
        return {
            languages: settingsContainer.sdlLanguagesList
        }
    }

    function changeRegistration (language) {
        dataContainer.changeRegistrationUI(language)
    }
}
