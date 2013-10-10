import QtQuick 2.0
import "Common.js" as Common

Item {
    function filter (strings, fields) {
// substrings for each allowed field
        var fieldSubstrings = {}
// this cycle concatenates allowed lines sorting them by field
        for (var stringIndex = 0; stringIndex < strings.length; ++stringIndex) {
            for (var fieldIndex = 0; fieldIndex < fields.length; ++fieldIndex) {
                var fieldName = strings[stringIndex].fieldName
                if (fieldName === fields[fieldIndex]) {
                    if (fieldSubstrings[fieldName] !== undefined) {
                        fieldSubstrings[fieldName] += "\n";
                        fieldSubstrings[fieldName] += strings[stringIndex].fieldText
                    }
                    else {
                        fieldSubstrings[fieldName] = strings[stringIndex].fieldText
                    }
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
/*
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
*/
        var tryAgainTime = alertWindow.alert(fieldSubstrings, duration, appID)
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
            Common.TextFieldName.statusBar,
            Common.TextFieldName.mediaClock
        ]
        var fieldSubstrings = filter(showStrings, showFields)
        if (fieldSubstrings[Common.TextFieldName.mainField1] !== undefined) {
            dataContainer.hmiUIText.mainField1 = fieldSubstrings[Common.TextFieldName.mainField1]
        }
        if (fieldSubstrings[Common.TextFieldName.mainField2] !== undefined) {
            dataContainer.hmiUIText.mainField2 = fieldSubstrings[Common.TextFieldName.mainField2]
        }
        if (fieldSubstrings[Common.TextFieldName.mainField3] !== undefined) {
            dataContainer.hmiUIText.mainField3 = fieldSubstrings[Common.TextFieldName.mainField3]
        }
        if (fieldSubstrings[Common.TextFieldName.mainField4] !== undefined) {
            dataContainer.hmiUIText.mainField4 = fieldSubstrings[Common.TextFieldName.mainField4]
        }
        if (fieldSubstrings[Common.TextFieldName.statusBar] !== undefined) {
            dataContainer.hmiUIText.statusBar = fieldSubstrings[Common.TextFieldName.statusBar]
        }
        if (fieldSubstrings[Common.TextFieldName.mediaClock] !== undefined) {
            dataContainer.hmiUIText.mediaClock = fieldSubstrings[Common.TextFieldName.mediaClock]
        }
        if (alignment !== undefined) {
            switch (alignment) {
                case Common.TextAlignment.LEFT_ALIGNED:
                    dataContainer.hmiUITextAlignment = Text.AlignLeft
                    break;
                case Common.TextAlignment.RIGHT_ALIGNED:
                    dataContainer.hmiUITextAlignment = Text.AlignRight
                    break;
                case Common.TextAlignment.CENTERED:
                    dataContainer.hmiUITextAlignment = Text.AlignHCenter
                    break;
                default:
                    dataContainer.hmiUITextAlignment = Text.AlignHCenter
                    break;
            }
        }
        else {
            dataContainer.hmiUITextAlignment = Text.AlignHCenter
        }

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
