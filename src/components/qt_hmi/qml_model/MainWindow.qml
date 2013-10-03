import QtQuick 2.0
import com.ford.sdl.hmi.dbus_adapter 1.0
import com.ford.sdl.hmi.hw_buttons 1.0
import "./controls"
import "./views"
import "./hmi_api" as HmiApi
import "./models"
import "./popups"

Rectangle {
    width: 1600
    height: 768
    property string startQml: "./views/AMPlayerView.qml"
    property int margin: 20
    property int minWidth: 800
    property int minHeight: 600
    color: "black"

    DataStorage {
        id: dataContainer

        onHmiContextChanged: {
            // TODO: Send notification to SDL
        }
    }

    SettingsStorage {
        id: settingsContainer
    }

    Item {
        id: mainScreen
        anchors.top: parent. top
        anchors.left: parent.left
        width: (parent.width * 0.62 < minWidth) ? minWidth : (parent.width * 0.62)
        height: (parent.height < minHeight) ? minHeight : parent.height
        visible: false

        Item {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.25
            width: parent.width
            HeaderMenu {}
        }

        Item {
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.bottomMargin: 30
            anchors.fill: parent

            Loader {
                id: contentLoader
                asynchronous: true
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height * 0.75
                width: parent.width
                source:startQml
                property var viewTransitionStack : []

                function reset(){
                    viewTransitionStack = []
                    source = startQml
                }

                function go(path) {
                    viewTransitionStack.push(source.toString())
                    source = path
                }

                function back() {
                    source = viewTransitionStack.pop()
                }

                onStatusChanged: {
                    if (status == Component.Ready) {
                        dataContainer.hmiContext = item.context
                    }
                }
            }
        }
    }

    Item {
        id: hwBtnScreen
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: mainScreen.right
        width: parent.width * 0.38
        height: (parent.height < minHeight) ? minHeight : parent.height
        HardwareButtonsView {}
    }

    HMIProxy {
        HmiApi.ButtonsProxy {
            id: sdlButtons
            objectName: "Buttons"
        }
        HmiApi.BasicCommunicationProxy {
            id: sdlBasicCommunication
            objectName: "BasicCommunication"
        }
        HmiApi.VRProxy {
            id: sdlVR
            objectName: "VR"
        }
        HmiApi.TTSProxy {
            id: sdlTTS
            objectName: "TTS"
        }
        HmiApi.NavigationProxy {
            id: sdlNavigation
            objectName: "Navigation"
        }
        HmiApi.VehicleInfoProxy {
            id: sdlVehicleInfo
            objectName: "VehicleInfo"
        }
        HmiApi.UIProxy {
            id: sdlUI
            objectName: "UI"
        }
    }

    SDLProxy {
        id: sdlProxy

        onAppRegistered: {
            dataContainer.applicationList.append(
            {
                 appName: application.appName,
                 ngnMediaScreenAppName: application.ngnMediaScreenAppName,
                 icon: application.icon,
                 deviceName: application.deviceName,
                 appId: application.appID,
                 hmiDisplayLanguageDesired: application.hmiDisplayLanguageDesired,
                 isMediaApplication: application.isMediaApplication,
                 appType: application.appType
             })
        }
    }

    AlertWindow {
        id: alertWindow
        objectName: "AlertWindow"
    }

    Component.onCompleted: {
        dataContainer.hmiVRAvailable = true
        dataContainer.hmiTTSAvailable = true
        dataContainer.hmiNavigationAvailable = true
        dataContainer.hmiVehicleInfoAvailable = true
        dataContainer.hmiUIAvailable = true

        sdlBasicCommunication.onReady()
    }
}
