import QtQuick 2.0
import ".."

Item {
  BasicCommunication {
    id: sdlBasicCommunication
  }
  function updateDeviceList(params) {
     return sdlBasicCommunication.updateDeviceList(params.deviceList)
  }

  function allowDeviceToConnect(params) {
     return sdlBasicCommunication.allowDeviceToConnect(params.device)
  }

  function updateAppList(params) {
     return sdlBasicCommunication.updateAppList(params.applications)
  }

  function mixingAudioSupported(params) {
     return sdlBasicCommunication.mixingAudioSupported()
  }

  function allowAllApps(params) {
     return sdlBasicCommunication.allowAllApps()
  }

  function allowApp(params) {
     return sdlBasicCommunication.allowApp(params.app, params.appPermissions)
  }

  signal onReady()
  signal onStartDeviceDiscovery()
  signal onUpdateDeviceList()
  signal onDeviceChosen(var deviceInfo)
  signal onFindApplications(var deviceInfo)
  signal onAppActivated(int appID)
  signal onAppDeactivated(int appID, int reason)
  signal onAppRegistered(var application)
  signal onAppUnregistered(int appID)
  signal onExitApplication(int appID)
  signal onExitAllApplications(int reason)
  signal playTone()
}
