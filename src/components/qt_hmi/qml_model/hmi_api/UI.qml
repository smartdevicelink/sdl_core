import QtQuick 2.0
import "Common.js" as Common
import "Async.js" as Async

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
        return interactionModel.performInteraction(initialText, choiceSet, vrHelp, timeout, appID)
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
        dataContainer.setApplicationProperties(appID, { icon: syncFileName.value })
    }

    function slider (numTicks, position, sliderHeader, sliderFooter, timeout, appID) {
        console.debug("enter",numTicks, position, sliderHeader, sliderFooter, timeout, appID)
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

        sliderPopup.showSlider()
        sliderPopup.async = new Async.AsyncCall();
        console.debug("exit")
        return sliderPopup.async;
    }

    function scrollableMessage (messageText, timeout, softButtons, appID) {
        console.debug("scrollableMessage ", messageText, timeout, softButtons, appID)
        if(dataContainer.scrollableMessageModel.running){
            //send error response if long message already running
            console.debug("scrollableMessage throw")
            throw Common.Result.ABORTED
        }

        if(messageText !== undefined) {
            dataContainer.scrollableMessageModel.longMessageText = messageText.fieldText
        }
        if (softButtons !== undefined) {
            dataContainer.scrollableMessageModel.softButtons.clear();
            softButtons.forEach(fillSoftButtons, dataContainer.scrollableMessageModel.softButtons);
        }
        if(timeout !== undefined) {
            dataContainer.scrollableMessageModel.timeout = timeout
        }
        if(appID !== undefined) {
            dataContainer.scrollableMessageModel.appId = appID
        }
        dataContainer.scrollableMessageModel.async = new Async.AsyncCall()
        contentLoader.go("./views/ScrollableMessageView.qml")
        console.debug("scrollableMessage exit")
        return dataContainer.scrollableMessageModel.async
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
