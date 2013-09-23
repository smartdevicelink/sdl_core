import QtQuick 2.0
import com.ford.hmi_framework 1.0
import sdl.core.api 1.0
import "./controls"
import "./views"
import "./hmi_api" as HmiApi
import "./models"

Rectangle{
    width: 1600
    height: 768
    property string startQml: "./views/AMPlayerView.qml"
    property int margin: 20
    property int minWidth: 800
    property int minHeight: 600
    color: "black"

    DataStorage{
        id: dataContainer
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
                property var screenMovingStack : []

                function go(path) {
                    screenMovingStack.push(source.toString())
                    source = path
                }

                function back() {
                    source = screenMovingStack.pop()
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

    Api {
        HmiApi.Buttons {
            id: sdlButtons
            objectName: "Buttons"
        }
        HmiApi.BasicCommunication {
            id: sdlBasicCommunications
            objectName: "BasicCommunications"
        }
        HmiApi.VR {
            id: sdlVR
            objectName: "VR"
        }
        HmiApi.TTS {
            id: sdlTTS
            objectName: "TTS"
        }
        HmiApi.Navigation {
            id: sdlNavigation
            objectName: "Navigation"
        }
        HmiApi.VehicleInfo {
            id: sdlVehicleInfo
            objectName: "VehicleInfo"
        }
        HmiApi.UI {
            id: sdlUI
            objectName: "UI"
        }
    }

    Component.onCompleted: {
        sdlVR.available = true
        sdlTTS.available = true
        sdlNavigation.available = true
        sdlVehicleInfo.available = true
        sdlUI.available = true
        sdlBasicCommunications.onReady()
    }
}
