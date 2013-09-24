import QtQuick 2.0
import ".."
Item {
  UI {
    id: sdlUI
  }
  function alert(params) {
     return sdlUI.alert(params.alertStrings, params.duration, params.softButtons, params.appID)
  }

  function show(params) {
     return sdlUI.show(params.showStrings, params.alignment, params.graphic, params.softButtons, params.customPresets, params.appID)
  }

  function addCommand(params) {
     return sdlUI.addCommand(params.cmdID, params.menuParams, params.cmdIcon, params.appID)
  }

  function deleteCommand(params) {
     return sdlUI.deleteCommand(params.cmdID, params.appID)
  }

  function addSubMenu(params) {
     return sdlUI.addSubMenu(params.menuID, params.menuParams, params.appID)
  }

  function deleteSubMenu(params) {
     return sdlUI.deleteSubMenu(params.menuID, params.appID)
  }

  function performInteraction(params) {
     return sdlUI.performInteraction(params.initialText, params.choiceSet, params.vrHelp, params.timeout, params.appID)
  }

  function setMediaClockTimer(params) {
     return sdlUI.setMediaClockTimer(params.startTime, params.updateMode, params.appID)
  }

  function setGlobalProperties(params) {
     return sdlUI.setGlobalProperties(params.vrHelpTitle, params.vrHelp, params.appID)
  }

  function showVrHelp(params) {
     return sdlUI.showVrHelp(params.vrHelpTitle, params.vrHelp, params.appID)
  }

  function getCapabilities(params) {
     return sdlUI.getCapabilities()
  }

  function changeRegistration(params) {
     return sdlUI.changeRegistration(params.language, params.appID)
  }

  function getSupportedLanguages(params) {
     return sdlUI.getSupportedLanguages()
  }

  function getLanguage(params) {
     return sdlUI.getLanguage()
  }

  function setAppIcon(params) {
     return sdlUI.setAppIcon(params.syncFileName, params.appID)
  }

  function slider(params) {
     return sdlUI.slider(params.numTicks, params.position, params.sliderHeader, params.sliderFooter, params.timeout, params.appID)
  }

  function scrollableMessage(params) {
     return sdlUI.scrollableMessage(params.messageText, params.timeout, params.softButtons, params.appID)
  }

  function performAudioPassThru(params) {
     return sdlUI.performAudioPassThru(params.audioPassThruDisplayTexts, params.maxDuration)
  }

  function endAudioPassThru(params) {
     return sdlUI.endAudioPassThru()
  }

  function isReady(params) {
     return sdlUI.isReady()
  }

  function closePopUp(params) {
     return sdlUI.closePopUp()
  }

  signal showNotification(var text, var icon, int timeout)
  signal onCommand(int cmdID, int appID)
  signal onSystemContext(int systemContext)
  signal onLanguageChange(int language)
  signal onDriverDistraction(int state)
}
