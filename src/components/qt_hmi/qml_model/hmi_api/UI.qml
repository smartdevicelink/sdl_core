import QtQuick 2.0
import "Common.js" as Common
import "Async.js" as Async
import "../models/Internal.js" as Internal

Item {
    function filter (strings, fields) {
        console.debug("enter")
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
        console.debug("exit")
        return fieldSubstrings
    }

    function alert (alertStrings, duration, softButtons, progressIndicator, appID) {
        var fieldSubstrings = alertStrings
	    .sort(function(a, b) { return a.fieldName - b.fieldName }) // sorting by fieldName
	    .map(function(val) { return val.fieldText });              // mapping to array of strings

        var tryAgainTime = alertWindow.alert(fieldSubstrings, duration, progressIndicator, softButtons, appID)
        if (tryAgainTime === undefined) {
            alertWindow.async = new Async.AsyncCall();
            return alertWindow.async;
        }
        else {
            return { tryAgainTime: tryAgainTime }
        }
    }

    function show (showStrings, alignment, graphic, softButtons, customPresets, appID) {
        console.debug("enter: " + showStrings + ", " + alignment + ", " + graphic + ", "+ softButtons + ", " + customPresets + ", " + appID)
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
        console.debug("exit")
    }

    function addCommand (cmdID, menuParams, cmdIcon, appID) {
        dataContainer.addCommand(cmdID, menuParams, cmdIcon, appID)
    }

    function deleteCommand (cmdID, appID) {
        dataContainer.deleteCommand(cmdID, appID)
    }

    function addSubMenu (menuID, menuParams, appID) {
        dataContainer.addSubMenu(menuID, menuParams, appID)
    }

    function deleteSubMenu (menuID, appID) {
        dataContainer.deleteSubMenu(menuID, appID)
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
        var newUpdateMode
        var newRunningMode
        var newMagic
        var newTotal
        switch (updateMode) {
            case Common.ClockUpdateMode.COUNTUP:
                newUpdateMode = Internal.MediaClockUpdateMode.MCU_COUNTUP
                newRunningMode = Internal.MediaClockRunningMode.MCR_RUNNING
                newMagic = secondsSinceEpoch - hmsTime
                newTotal = 0
                break
            case Common.ClockUpdateMode.COUNTDOWN:
                newUpdateMode = Internal.MediaClockUpdateMode.MCU_COUNTDOWN
                newRunningMode = Internal.MediaClockRunningMode.MCR_RUNNING
                newMagic = secondsSinceEpoch + hmsTime
                newTotal = hmsTime
                break
            case Common.ClockUpdateMode.PAUSE:
                var mediaClockMagic = dataContainer.getApplication(appID).mediaClock.magic
                newUpdateMode = dataContainer.getApplication(appID).mediaClock.updateMode
                switch (newUpdateMode) {
                    case Internal.MediaClockUpdateMode.MCU_COUNTUP:
                        newMagic = secondsSinceEpoch - mediaClockMagic
                        break
                    case Internal.MediaClockUpdateMode.MCU_COUNTDOWN:
                        newMagic = mediaClockMagic - secondsSinceEpoch
                        break
                }
                newRunningMode = Internal.MediaClockRunningMode.MCR_STOPPED
                newTotal = dataContainer.getApplication(appID).mediaClock.total
                break
            case Common.ClockUpdateMode.RESUME:
                newUpdateMode = dataContainer.getApplication(appID).mediaClock.updateMode
                var mediaClockTime = dataContainer.getApplication(appID).mediaClock.magic
                switch (newUpdateMode) {
                    case Internal.MediaClockUpdateMode.MCU_COUNTUP:
                        newMagic = secondsSinceEpoch - mediaClockTime
                        break
                    case Internal.MediaClockUpdateMode.MCU_COUNTDOWN:
                        newMagic = secondsSinceEpoch + mediaClockTime
                        break
                }
                newRunningMode = Internal.MediaClockRunningMode.MCR_RUNNING
                newTotal = dataContainer.getApplication(appID).mediaClock.total
                break
            case Common.ClockUpdateMode.CLEAR:
                newUpdateMode = Internal.MediaClockUpdateMode.MCU_COUNTUP
                newRunningMode = Internal.MediaClockRunningMode.MCR_STOPPED
                newMagic = 0
                newTotal = 0
                break
        }
        dataContainer.setApplicationProperties(appID, {
            "mediaClock": {
                "updateMode": newUpdateMode,
                "runningMode": newRunningMode,
                "magic": newMagic,
                "total": newTotal
            }
        })
        console.debug("exit")
    }

    function setGlobalProperties (vrHelpTitle, vrHelp, menuTitle, menuIcon, keyboardProperties, appID) {
        console.debug("enter: " + vrHelpTitle + ", " + vrHelp + ", " + menuTitle + ", " + menuIcon + ", " + keyboardProperties + ", " + appID)
        if (vrHelpTitle) {
            vrHelpPopup.title = vrHelpTitle
        }
        dataContainer.setVrHelp(vrHelp)
        console.debug("exit")
    }

    function showVrHelp (vrHelpTitle, vrHelp, appID) {
        console.debug("enter: " + vrHelpTitle + ", " + vrHelp + ", " + appID)
        if (vrHelpTitle) {
            vrHelpPopup.title = vrHelpTitle
        }
        dataContainer.setVrHelp(vrHelp)
        vrHelpPopup.activate()
        console.debug("exit")
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

    function changeRegistration (language, appID) {
        dataContainer.changeRegistrationUI(language, appID)
    }

    function setAppIcon (syncFileName, appID) {
        dataContainer.setApplicationProperties(appID, { icon: syncFileName.value })
    }

    function slider (numTicks, position, sliderHeader, sliderFooter, timeout, appID) {
        console.debug("enter", numTicks, position, sliderHeader, sliderFooter, timeout, appID)
        if (dataContainer.uiSlider.running) {
            console.debug("aborted")
            throw Common.Result.ABORTED
        }

        dataContainer.uiSlider.appName = dataContainer.getApplication(appID).appName
        dataContainer.uiSlider.header = sliderHeader
        dataContainer.uiSlider.footer = sliderFooter
        dataContainer.uiSlider.numTicks = numTicks
        dataContainer.uiSlider.position = position
        dataContainer.uiSlider.timeout = timeout

        if (timeout !== 0) {
            sliderPopup.showSlider()
            sliderPopup.async = new Async.AsyncCall();
            console.debug("exit")
            return sliderPopup.async;
        } else {
            return { sliderPosition: position }
        }
    }

    function scrollableMessage (messageText, timeout, softButtons, appID) {
        console.debug("scrollableMessage ", messageText, timeout, softButtons, appID)
        if(dataContainer.scrollableMessageModel.running){
            //send error response if long message already running
            console.debug("scrollableMessage throw")
            throw Common.Result.ABORTED
        }

        dataContainer.scrollableMessageModel.longMessageText = messageText.fieldText
        dataContainer.scrollableMessageModel.softButtons.clear();
        if (softButtons !== undefined) {
            softButtons.forEach(fillSoftButtons, dataContainer.scrollableMessageModel.softButtons);
        }
        dataContainer.scrollableMessageModel.timeout = timeout
        dataContainer.scrollableMessageModel.appId = appID
        dataContainer.scrollableMessageModel.async = new Async.AsyncCall()
        contentLoader.go("./views/ScrollableMessageView.qml")
        console.debug("scrollableMessage exit")
        return dataContainer.scrollableMessageModel.async
    }

    function getCapabilities() {
        return {
            "audioPassThruCapabilities": {
                "samplingRate": Common.SamplingRate.RATE_44KHZ,
                "bitsPerSample": Common.BitsPerSample.RATE_8_BIT,
                "audioType": Common.AudioType.PCM
            },
            "displayCapabilities": settingsContainer.displayCapabilities,
            "hmiZoneCapabilities": Common.HmiZoneCapabilities.FRONT,
            "softButtonCapabilities": settingsContainer.softButtonCapabilities
        }
    }

    function performAudioPassThru (audioPassThruDisplayTexts, timeout, appID) {
        console.debug("enter", audioPassThruDisplayTexts, timeout)

        if (dataContainer.uiAudioPassThru.running) {
            console.debug("aborted")
            throw Common.Result.ABORTED
        }

        dataContainer.uiAudioPassThru.appName = dataContainer.getApplication(appID).appName
        dataContainer.uiAudioPassThru.timeout = timeout
        if (audioPassThruDisplayTexts.length === 2) {
            dataContainer.uiAudioPassThru.firstLine = audioPassThruDisplayTexts[0].fieldText
            dataContainer.uiAudioPassThru.secondLine = audioPassThruDisplayTexts[1].fieldText
        }
        performAudioPassThruPopup.async = new Async.AsyncCall();
        performAudioPassThruPopup.showAudioPassThru()
        console.debug("exit")
        return performAudioPassThruPopup.async;
    }

    function endAudioPassThru () {
        console.debug("enter")
        if (!dataContainer.uiAudioPassThru.running) {
            console.debug("rejected")
            throw Common.Result.REJECTED
        }
        performAudioPassThruPopup.complete(Common.Result.SUCCESS)
        console.debug("exit")
    }

    function closePopUp () {
        console.debug("enter")
        if (dataContainer.activePopup) {
            //close pop-up that is currently active with ABORT code
            dataContainer.activePopup.complete(Common.Result.ABORT)
        }
        //response to this callwith SUCCESS code
        console.debug("exit")
    }

    function fillSoftButtons(element, index, array) {
        this.append({
                        type: element.type,
                        name: element.text,
                        image: element.image,
                        isHighlighted: element.isHighlighted,
                        buttonId: element.softButtonID,
                        action: element.systemAction
                    });
    }

}
