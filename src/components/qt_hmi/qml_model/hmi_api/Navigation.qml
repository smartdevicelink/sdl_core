import QtQuick 2.0
import "Common.js" as Common

Item {
    function isReady () {
        return {
            available: dataContainer.hmiNavigationAvailable
        }
    }

    function showConstantTBT(navigationTexts, turnIcon, distanceToManeuver,
                             distanceToManeuverScale, maneuverComplete,
                             softButtons, appID) {
        console.log("Method call");
        navigationTexts.forEach(fillTexts);
        if (turnIcon !== undefined) {
            fillIcon(turnIcon);
        }
        dataContainer.navigationModel.distanceToManeuver = distanceToManeuver;
        dataContainer.navigationModel.distanceToManeuverScale = distanceToManeuverScale;
        if (maneuverComplete !== undefined) {
            dataContainer.navigationModel.maneuverComplete = maneuverComplete;
        }
        if (softButtons !== undefined) {
            dataContainer.navigationModel.softButtons.clear();
            softButtons.forEach(fillSoftButtons);
        }
        dataContainer.navigationModel.appId = appID;
        contentLoader.go("./views/TurnByTurnView.qml");
        console.log("Method return");
    }

    function alertManeuver(softButtons) {

    }

    function updateTurnList(turnList, softButtons, appID) {

    }

    function fillTexts(element, index, array) {
        switch (element.fieldName) {
        case Common.TextFieldName.navigationText1:
            dataContainer.navigationModel.text1 = element.fieldText;
            break;
        case Common.TextFieldName.navigationText2:
            dataContainer.navigationModel.text2 = element.fieldText;
            break;
        case Common.TextFieldName.ETA:
            dataContainer.navigationModel.eta = element.fieldText;
            break;
        case Common.TextFieldName.totalDistance:
            dataContainer.navigationModel.totalDistance = element.fieldText;
            break;
        }
    }

    function fillIcon(turnIcon) {
        switch (turnIcon.imageType) {
        case Common.ImageType.STATIC:
            dataContainer.navigationModel.pathIcon = turnIcon.value;
            break;
        case Common.ImageType.DYNAMIC:
            dataContainer.navigationModel.imageIcon = turnIcon.value;
            break;
        }
    }

    function fillSoftButtons(element, index, array) {
        dataContainer.navigationModel.softButtons.append(
                    {
                        type: element.type,
                        text: element.text,
                        image: element.image,
                        highlighted: element.isHighlighted,
                        buttonId: element.softButtonID,
                        action: element.systemAction
                    });
    }
}
