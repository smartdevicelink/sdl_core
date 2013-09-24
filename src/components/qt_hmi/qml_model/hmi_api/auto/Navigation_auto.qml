import QtQuick 2.0
import ".."
Item {
  Navigation {
    id: sdlNavigation
  }
  function isReady(params) {
     return sdlNavigation.isReady()
  }

  function showConstantTBT(params) {
     return sdlNavigation.showConstantTBT(params.navigationTexts, params.turnIcon, params.distanceToManeuver, params.distanceToManeuverScale, params.maneuverComplete, params.softButtons, params.appID)
  }

  function alertManeuver(params) {
     return sdlNavigation.alertManeuver(params.softButtons)
  }

  function updateTurnList(params) {
     return sdlNavigation.updateTurnList(params.turnList, params.softButtons, params.appID)
  }

  signal onTBTClientState(int state)
}
