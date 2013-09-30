import QtQuick 2.0
import com.ford.sdl.hmi.dbus_adapter 1.0
import com.ford.sdl.hmi.hw_buttons 1.0
import "./controls"
import "./views"
import "./hmi_api" as HmiApi
import "./models"
import "./popups"

Rectangle{
    width: 1600
    height: 768
    property string startQml: "./views/AMPlayerView.qml"
    property int margin: 20
    property int minWidth: 800
    property int minHeight: 600
    color: "black"

    DataStorage {
        id: dataContainer
    }
    SettingsStorage {
        id: settings
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

        Item{
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.25
            width: parent.width
            HeaderMenu{}
        }

        Item{
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.bottomMargin: 30
            anchors.fill: parent

            Loader {
                id: contentLoader
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
        HmiApi.Buttons_proxy {
            id: sdlButtons
            objectName: "Buttons"
        }
        HmiApi.BasicCommunication_proxy {
            id: sdlBasicCommunications
            objectName: "BasicCommunication"
        }
        HmiApi.VR_proxy {
            id: sdlVR
            objectName: "VR"
        }
        HmiApi.TTS_proxy {
            id: sdlTTS
            objectName: "TTS"
        }
        HmiApi.Navigation_proxy {
            id: sdlNavigation
            objectName: "Navigation"
        }
        HmiApi.VehicleInfo_proxy {
            id: sdlVehicleInfo
            objectName: "VehicleInfo"
        }
        HmiApi.UI_proxy {
            id: sdlUI
            objectName: "UI"
        }
    }

    SDLProxy {
        id: sdlProxy

        onAppRegistered: {
            console.log("new app registered")
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

        sdlBasicCommunications.onReady()
    }
}
