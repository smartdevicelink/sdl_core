/**
 * @file UI.qml
 * @brief UI interface implementation.
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

import QtQuick 1.1
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
            return {
                "__retCode": Common.Result.REJECTED,
                "tryAgainTime": tryAgainTime
            }
        }
    }

    function show (showStrings, alignment, graphic, softButtons, customPresets, appID) {
        console.debug("enter: " + showStrings + ", " + alignment + ", " + graphic + ", "+ softButtons + ", " + customPresets + ", " + appID)
        var app = dataContainer.getApplication(appID);

        if (softButtons) {
            app.softButtons.clear()
            softButtons.forEach(function(x) { app.softButtons.append(x); });
        }

        if (customPresets) {
            app.customPresets.clear()
            customPresets.forEach( function(x) { app.customPresets.append( {text: x} ); } )
        }

        var textAlignment
        switch (alignment) {
            case Common.TextAlignment.LEFT_ALIGNED:
                textAlignment = Text.AlignLeft
                break;
            case Common.TextAlignment.RIGHT_ALIGNED:
                textAlignment = Text.AlignRight
                break;
            case Common.TextAlignment.CENTERED:
                textAlignment = Text.AlignHCenter
                break;
            default:
                textAlignment = Text.AlignHCenter
                break;
        }

        // with this array we grab only the lines we need
        var showFields = [
            Common.TextFieldName.mainField1,
            Common.TextFieldName.mainField2,
            Common.TextFieldName.mainField3,
            Common.TextFieldName.mainField4,
            Common.TextFieldName.statusBar,
            Common.TextFieldName.mediaClock,
            Common.TextFieldName.mediaTrack
        ]
        var fieldSubstrings = filter(showStrings, showFields)
        var showData = { hmiUIText: {} }
        if (fieldSubstrings[Common.TextFieldName.mainField1] !== undefined) { showData.hmiUIText.mainField1 = fieldSubstrings[Common.TextFieldName.mainField1]; }
        if (fieldSubstrings[Common.TextFieldName.mainField2] !== undefined) { showData.hmiUIText.mainField2 = fieldSubstrings[Common.TextFieldName.mainField2]; }
        if (fieldSubstrings[Common.TextFieldName.mainField3] !== undefined) { showData.hmiUIText.mainField3 = fieldSubstrings[Common.TextFieldName.mainField3]; }
        if (fieldSubstrings[Common.TextFieldName.mainField4] !== undefined) { showData.hmiUIText.mainField4 = fieldSubstrings[Common.TextFieldName.mainField4]; }
        if (fieldSubstrings[Common.TextFieldName.statusBar] !== undefined) { showData.hmiUIText.statusBar = fieldSubstrings[Common.TextFieldName.statusBar]; }
        if (fieldSubstrings[Common.TextFieldName.mediaTrack] !== undefined) { showData.hmiUIText.mediaTrack = fieldSubstrings[Common.TextFieldName.mediaTrack]; }
        if (graphic) { showData.hmiUIText.image = graphic.value; }
        if (textAlignment) { showData.hmiUITextAlignment = textAlignment; }
        if (fieldSubstrings[Common.TextFieldName.mediaClock]) {
            showData.mediaClock = {
                "updateMode": Internal.MediaClockUpdateMode.MCU_COUNTUP,
                "runningMode": Internal.MediaClockRunningMode.MCR_STOPPED,
                "magic": Internal.stringToHmsTime(fieldSubstrings[Common.TextFieldName.mediaClock]),
                "total": 0
            }
        }

        dataContainer.setApplicationProperties(appID, showData);
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

    function setMediaClockTimer (startTime, endTime, updateMode, appID) {
        console.debug("enter: {", startTime, ", ", endTime, ", ", updateMode, ", ", appID, "}")

        var app = dataContainer.getApplication(appID)
        var newStartTime
        var newEndTime
        var newUpdateMode
        var newRunningMode
        var newStartTimeForProgress
        var resultCode
        var sendErrorResponce = false

        switch (updateMode) {
            case Common.ClockUpdateMode.COUNTUP:
                if (startTime === undefined) {
                    resultCode = Common.Result.INVALID_DATA
                    sendErrorResponce = true
                    break
                }
                newStartTime = Internal.hmsTime(startTime.hours, startTime.minutes, startTime.seconds)
                newEndTime = endTime ? Internal.hmsTime(endTime.hours, endTime.minutes, endTime.seconds) : -1
                newUpdateMode = Internal.MediaClockUpdateMode.MCU_COUNTUP
                newRunningMode = Internal.MediaClockRunningMode.MCR_RUNNING
                newStartTimeForProgress = Internal.hmsTime(startTime.hours, startTime.minutes, startTime.seconds)
                resultCode = Common.Result.SUCCESS
                break

            case Common.ClockUpdateMode.COUNTDOWN:
                if (startTime === undefined) {
                    resultCode = Common.Result.INVALID_DATA
                    sendErrorResponce = true
                    break
                }
                newStartTime = Internal.hmsTime(startTime.hours, startTime.minutes, startTime.seconds)
                newEndTime = endTime ? Internal.hmsTime(endTime.hours, endTime.minutes, endTime.seconds) : -1
                newUpdateMode = Internal.MediaClockUpdateMode.MCU_COUNTDOWN
                newRunningMode = Internal.MediaClockRunningMode.MCR_RUNNING
                newStartTimeForProgress = Internal.hmsTime(startTime.hours, startTime.minutes, startTime.seconds)
                resultCode = Common.Result.SUCCESS
                break

            case Common.ClockUpdateMode.PAUSE:
                // Already paused or cleared
                if ((app.mediaClock.runningMode === Internal.MediaClockRunningMode.MCR_STOPPED)
                        || (app.mediaClock.startTime === -1 && app.mediaClock.endTime === -1)) {
                    resultCode = Common.Result.IGNORED
                    sendErrorResponce = true
                    break
                }
                newStartTime = app.mediaClock.startTime
                newEndTime = app.mediaClock.endTime
                newRunningMode = Internal.MediaClockRunningMode.MCR_STOPPED
                newUpdateMode = app.mediaClock.updateMode
                newStartTimeForProgress = app.mediaClock.startTime
                resultCode = Common.Result.SUCCESS
                break

            case Common.ClockUpdateMode.RESUME:
                // Already resumed or cleared
                if ((app.mediaClock.runningMode === Internal.MediaClockRunningMode.MCR_RUNNING)
                        || (app.mediaClock.startTime === -1 && app.mediaClock.endTime === -1)) {
                    resultCode = Common.Result.IGNORED
                    sendErrorResponce = true
                    break
                }
                newStartTime = app.mediaClock.startTime
                newEndTime = app.mediaClock.endTime
                newRunningMode = Internal.MediaClockRunningMode.MCR_RUNNING
                newStartTimeForProgress = app.mediaClock.startTime
                newUpdateMode = app.mediaClock.updateMode
                resultCode = Common.Result.SUCCESS
                break

            case Common.ClockUpdateMode.CLEAR:
                newStartTime = -1
                newEndTime = -1
                newRunningMode = Internal.MediaClockRunningMode.MCR_STOPPED
                newUpdateMode = app.mediaClock.updateMode
                newStartTimeForProgress = -1
                resultCode = Common.Result.SUCCESS
                break
        }

        if (sendErrorResponce) {
            console.debug("exit with result code: ", resultCode)
            return { __retCode: resultCode }
        }

        dataContainer.setApplicationProperties(appID, {
            "mediaClock": {
                "updateMode": newUpdateMode,
                "runningMode": newRunningMode,
                "startTime": newStartTime,
                "endTime": newEndTime,
                "startTimeForProgress": newStartTimeForProgress
            }
        })

        console.debug("exit")
        return { __retCode: resultCode }
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
        vrHelpPopup.show()
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
            return  {__retCode: Common.Result.ABORTED, sliderPosition: position}
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
        DBus.sendReply({__retCode: Common.Result.SUCCESS})
        performAudioPassThruPopup.complete(Common.Result.SUCCESS)
        console.debug("exit")
    }

    function closePopUp (methodName) {
        console.debug("enter")
        var popUpToClose

        if (dataContainer.activePopup.length === 0) {
            return { __retCode: Common.Result.ABORT, __message: "No active PopUps"}
        }

        if (methodName !== undefined) {
            popUpToClose = methodName
        } else {
            popUpToClose = dataContainer.activePopup[dataContain.activePopUp.length - 1]
        }

        switch (popUpToClose) {
            case "UI.PerformInteraction":
                interactionPopup.complete(Common.Result.SUCCESS)
                break
            case "UI.Slider":
                sliderPopup.complete(Common.Result.SUCCESS)
                break
            case "UI.PerformAudioPassThru":
                performAudioPassThruPopup.complete(Common.Result.SUCCESS)
                break
            case "UI.Alert":
                alertWindow.complete(Common.Result.SUCCESS)
                break
            case "UI.VrHelp":
                vrHelpPopup.complete(Common.Result.SUCCESS)
                break
        }
        console.debug("exit")
    }

    function fillSoftButtons(element, index, array) {
        this.append({
                        type: element.type,
                        text: element.text,
                        image: element.image,
                        isHighlighted: element.isHighlighted,
                        softButtonID: element.softButtonID,
                        systemAction: element.systemAction
                    });
    }

}
