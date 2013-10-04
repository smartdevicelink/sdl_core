import QtQuick 2.0
import "Common.js" as Common

Item {
    signal showNotification (var text, var icon, int timeout)
    signal onCommand (int cmdID, int appID)
    signal onSystemContext (int systemContext)
    signal onLanguageChange (int language)
    signal onDriverDistraction (int state)
    function filter (strings, fields) {
// substrings for each allowed field
        var fieldSubstrings = []
// have to populate the array to appropriate size
        for (var fieldIndex = 0; fieldIndex < fields.length; ++fieldIndex) {
            fieldSubstrings.push("")
        }
// this cycle concatenates allowed lines sorting them by field
        for (var stringIndex = 0; stringIndex < strings.length; ++stringIndex) {
            for (fieldIndex = 0; fieldIndex < fields.length; ++fieldIndex) {
                if (strings[stringIndex].fieldName === fields[fieldIndex]) {
                    if (fieldSubstrings[fieldIndex] !== "") {
                        fieldSubstrings[fieldIndex] += "\n";
                    }
                    fieldSubstrings[fieldIndex] += strings[stringIndex].fieldText
                }
            }
        }
        return fieldSubstrings
    }

    function alert (alertStrings, duration, softButtons, appID) {
// with this array we grab only the lines we need
        var alertFields = [Common.TextFieldName.alertText1,
                           Common.TextFieldName.alertText2,
                           Common.TextFieldName.alertText3]

        var fieldSubstrings = filter(alertStrings, alertFields)

        var alertString = ""
// this cycle concatenates all the substrings according to the order of the fields
        for (var fieldIndex = 0; fieldIndex < alertFields.length; ++fieldIndex) {
            if (fieldSubstrings[fieldIndex] !== "") {
                if (alertString !== "") {
                    alertString += "\n" // need linebreak
                }
                alertString += fieldSubstrings[fieldIndex]
            }
        }

        var tryAgainTime = alertWindow.alert(alertString, duration)
        if (tryAgainTime === undefined) {
            return {}
        }
        else {
            return {tryAgainTime: tryAgainTime}
        }
    }

    function show (showStrings, alignment, graphic, softButtons, customPresets, appID) {
// with this array we grab only the lines we need
        var showFields = [
            Common.TextFieldName.mainField1,
            Common.TextFieldName.mainField2,
            Common.TextFieldName.mainField3,
            Common.TextFieldName.mainField4,
            Common.TextFieldName.statusBar
        ]
        var fieldSubstrings = filter(showStrings, showFields)
        var fieldSubstringsStruct = {
            "mainField1": fieldSubstrings[0],
            "mainField2": fieldSubstrings[1],
            "mainField3": fieldSubstrings[2],
            "mainField4": fieldSubstrings[3],
            "statusBar": fieldSubstrings[4]
        }
        showWindow.show(fieldSubstringsStruct)
        return {}
    }

    function addCommand (cmdID, menuParams, cmdIcon, appID) {
    }

    function deleteCommand (cmdID, appID) {
    }

    function addSubMenu (menuID, menuParams, appID) {
    }

    function deleteSubMenu (menuID, appID) {
    }

    function performInteraction (initialText, choiceSet, vrHelp, timeout, appID) {
        return {}
    }

    function setMediaClockTimer (startTime, updateMode, appID) {
    }

    function setGlobalProperties (vrHelpTitle, vrHelp, appID) {
    }

    function showVrHelp (vrHelpTitle, vrHelp, appID) {
    }

    function getCapabilities () {
        return {
            displayCapabilities: settingsContainer.displayCapabilities,
            hmiZoneCapabilities: Common.HmiZoneCapabilities.FRONT,
            softButtonCapabilities: settingsContainer.softButtonCapabilities
        }
    }

    function isReady () {
        return {
            available: dataContainer.hmiUIAvailable
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

    function setAppIcon (syncFileName, appID) {
    }

    function slider (numTicks, position, sliderHeader, sliderFooter, timeout, appID) {
        return {
            sliderPosition: dataContainer.uiSliderPosition
        }
    }

    function scrollableMessage (messageText, timeout, softButtons, appID) {
    }

    function performAudioPassThru (audioPassThruDisplayTexts, maxDuration) {
    }

    function endAudioPassThru () {
    }

    function closePopUp () {
    }
}
