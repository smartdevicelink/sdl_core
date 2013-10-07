import QtQuick 2.0

Item {
    function isReady () {
        return {
            available: dataContainer.hmiNavigationAvailable
        }
    }

    function showConstantTBT(navigationTexts, turnIcon, distanceToManeuver,
                             distanceToManeuverScale, maneuverComplete,
                             softButtons, appID) {

    }

    function alertManeuver(softButtons) {

    }

    function updateTurnList(turnList, softButtons, appID) {

    }
}
