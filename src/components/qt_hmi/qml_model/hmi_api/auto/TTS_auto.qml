import QtQuick 2.0
import ".."

Item {
  TTS {
    id: sdlTTS
  }
  function isReady(params) {
     return sdlTTS.isReady()
  }

  function speak(params) {
     return sdlTTS.speak(params.ttsChunks, params.appID)
  }

  function stopSpeaking(params) {
     return sdlTTS.stopSpeaking()
  }

  function changeRegistration(params) {
     return sdlTTS.changeRegistration(params.language, params.appID)
  }

  function getSupportedLanguages(params) {
     return sdlTTS.getSupportedLanguages()
  }

  function getLanguage(params) {
     return sdlTTS.getLanguage()
  }

  function setGlobalProperties(params) {
     return sdlTTS.setGlobalProperties(params.helpPrompt, params.timeoutPrompt, params.appID)
  }

  signal onLanguageChange(int language)
}
