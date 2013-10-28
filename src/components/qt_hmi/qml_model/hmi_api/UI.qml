import QtQuick 2.0
import "Common.js" as Common
import "Async.js" as Async
import "../models/Internal.js" as Internal

Item {
    function filter (strings, fields) {
        console.debug("UI::filter(" + strings + ", " + fields + ")")
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
        console.debug("UI::filter(): exit")
        return fieldSubstrings
    }

    function alert (alertStrings, duration, softButtons, appID) {
// with this array we grab only the lines we need
        var alertFields = [Common.TextFieldName.alertText1,
                           Common.TextFieldName.alertText2,
                           Common.TextFieldName.alertText3]

        var fieldSubstrings = filter(alertStrings, alertFields)
        var tryAgainTime = alertWindow.alert(fieldSubstrings, duration, appID, softButtons)
        if (tryAgainTime === undefined) {
            alertWindow.async = new Async.AsyncCall();
            return alertWindow.async;
        }
        else {
            return { tryAgainTime: tryAgainTime }
        }
    }

    function show (showStrings, alignment, graphic, softButtons, customPresets, appID) {
        console.debug("UI::show(" + showStrings + ", " + alignment + ", " + graphic + ", "+ softButtons + ", " + customPresets + ", " + appID + ")")
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
        dataContainer.setApplicationProperties(
            appID, {
                "hmiUIText": {
                    "mainField1": fieldSubstrings[Common.TextFieldName.mainField1],
                    "mainField2": fieldSubstrings[Common.TextFieldName.mainField2],
                    "mainField3": fieldSubstrings[Common.TextFieldName.mainField3],
                    "mainField4": fieldSubstrings[Common.TextFieldName.mainField4],
                    "statusBar": fieldSubstrings[Common.TextFieldName.statusBar],
                    "mediaClock": fieldSubstrings[Common.TextFieldName.mediaClock],
                    "picture": fieldSubstrings[Common.TextFieldName.picture]
                  }
            }
        )
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
        console.debug("UI::show(): exit")
        return {}
    }

    function addCommand (cmdID, menuParams, cmdIcon, appID) {
        dataContainer.addCommand(cmdID, menuParams, cmdIcon, appID)
        return {}
    }

    function deleteCommand (cmdID, appID) {
        dataContainer.deleteCommand(cmdID, appID)
        return {}
    }

    function addSubMenu (menuID, menuParams, appID) {
        dataContainer.addSubMenu(menuID, menuParams, appID)
        return {}
    }

    function deleteSubMenu (menuID, appID) {
        dataContainer.deleteSubMenu(menuID, appID)
        return {}
    }

    function performInteraction (initialText, choiceSet, vrHelp, timeout, appID) {
        console.debug("enter")
        var async = dataContainer.interactionModel.performInteraction(initialText, choiceSet, vrHelp, timeout, appID)
        console.debug("exit")
        return async
    }

    function setMediaClockTimer (startTime, updateMode, appID) {
        console.debug("enter")
        var date = new Date()
        var secondsSinceEpoch = date.getTime() / 1000
        var hmsTime = Internal.hmsTime(startTime.hours, startTime.minutes, startTime.seconds)
        var mediaClockUpdateMode
        switch (updateMode) {
            case Common.ClockUpdateMode.COUNTUP:
                dataContainer.setApplicationProperties(appID, {
                    "mediaClock": {
                        "updateMode": Internal.MediaClockUpdateMode.MCU_COUNTUP,
                        "runningMode": Internal.MediaClockRunMode.MCR_RUNNING,
                        "magic": secondsSinceEpoch - hmsTime
                    }
                })
                break
            case Common.ClockUpdateMode.COUNTDOWN:
                dataContainer.setApplicationProperties(appID, {
                    "mediaClock": {
                        "updateMode": Internal.MediaClockUpdateMode.MCU_COUNTDOWN,
                        "runningMode": Internal.MediaClockRunMode.MCR_RUNNING,
                        "magic": secondsSinceEpoch + hmsTime
                    }
                })
                break
            case Common.ClockUpdateMode.PAUSE:
                var newMediaClockTime
                var mediaClockMagic = dataContainer.getApplication(appID).mediaClock.magic
                mediaClockUpdateMode = dataContainer.getApplication(appID).mediaClock.updateMode
                switch (mediaClockUpdateMode) {
                    case Internal.MediaClockUpdateMode.MCU_COUNTUP:
                        newMediaClockTime = secondsSinceEpoch - mediaClockMagic
                        break
                    case Internal.MediaClockUpdateMode.MCU_COUNTDOWN:
                        newMediaClockTime = mediaClockMagic - secondsSinceEpoch
                        break
                }
                dataContainer.setApplicationProperties(appID, {
                    "mediaClock": {
                        "updateMode": mediaClockUpdateMode,
                        "runningMode": Internal.MediaClockRunMode.MCR_STOPPED,
                        "magic": newMediaClockTime
                    }
                })
                break
            case Common.ClockUpdateMode.RESUME:
                mediaClockUpdateMode = dataContainer.getApplication(appID).mediaClock.updateMode
                var mediaClockTime = dataContainer.getApplication(appID).mediaClock.magic
                var newMediaClockTimeMagic
                switch (mediaClockUpdateMode) {
                    case Internal.MediaClockUpdateMode.MCU_COUNTUP:
                        newMediaClockTimeMagic = secondsSinceEpoch - mediaClockTime
                        break
                    case Internal.MediaClockUpdateMode.MCU_COUNTDOWN:
                        newMediaClockTimeMagic = secondsSinceEpoch + mediaClockTime
                        break
                }
                dataContainer.setApplicationProperties(appID, {
                    "mediaClock": {
                       "updateMode": mediaClockUpdateMode,
                       "runningMode": Internal.MediaClockRunMode.MCR_RUNNING,
                       "magic": newMediaClockTimeMagic
                    }
                })
                break
            case Common.ClockUpdateMode.CLEAR:
                dataContainer.setApplicationProperties(appID, {
                    "mediaClock": {
                        "updateMode": Internal.MediaClockUpdateMode.MCU_COUNTUP,
                        "runningMode": Internal.MediaClockRunMode.MCR_STOPPED,
                        "magic": 0
                    }
                })
                break;
        }
        console.debug("exit")
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
        dataContainer.setApplicationProperties(appID, { icon: syncFileName.value })
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
