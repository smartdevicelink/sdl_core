import QtQuick 2.0
import ".."
Item {
  VR {
    id: sdlVR
  }
  function isReady(params) {
     return sdlVR.isReady()
  }

  function addCommand(params) {
     return sdlVR.addCommand(params.cmdID, params.vrCommands, params.appID)
  }

  function deleteCommand(params) {
     return sdlVR.deleteCommand(params.cmdID, params.appID)
  }

  function changeRegistration(params) {
     return sdlVR.changeRegistration(params.language, params.appID)
  }

  function getSupportedLanguages(params) {
     return sdlVR.getSupportedLanguages()
  }

  function getLanguage(params) {
     return sdlVR.getLanguage()
  }

  signal started()
  signal stopped()
  signal onCommand(int cmdID, int appID)
  signal onLanguageChange(int language)
}
