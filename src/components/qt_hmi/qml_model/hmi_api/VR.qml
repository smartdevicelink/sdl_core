import QtQuick 2.0
import "Common.js" as Common

Item {
    function isReady() {
        return {
            available: dataContainer.hmiVRAvailable
        }
    }

    function addCommand(cmdID, vrCommands, appID) {
        console.log("VR.AddCommand: cmd ", cmdID, ", app ", appID);
        for (var i = 0; i < vrCommands.length; ++i) {
            dataContainer.vrCommands.append({
                                               cmdID: cmdID,
                                               command: vrCommands[i],
                                               appID: appID
                                           })
        }
    }

    function deleteCommand(cmdID, appID) {
        console.log("VR.DeleteCommand: cmd ", cmdID, ", app ", appID);
        for (var i = 0; i < dataContainer.vrCommands.count; ) {
            if ((dataContainer.vrCommands.get(i).cmdID === cmdID) &&
                    ((appID === undefined) || (dataContainer.vrCommands.get(i).appID === appID))) {
                dataContainer.vrCommands.remove(i);
                continue;
            }
            ++i;
        }
    }

    function getLanguage() {
        return {
            language: dataContainer.hmiTTSVRLanguage
        }
    }

    function getSupportedLanguages() {
        return {
            languages: settingsContainer.sdlLanguagesList
        }
    }

    function getCapabilities() {
        return {
            vrCapabilities: [ Common.VrCapabilities.VR_TEXT ]
        }
    }

    function changeRegistration(language, appID) {
        dataContainer.changeRegistrationTTSVR(language, appID);
    }
}
