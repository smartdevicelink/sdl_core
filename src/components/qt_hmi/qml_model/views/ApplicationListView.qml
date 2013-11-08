import QtQuick 2.0
import "../controls"
import "../models"
import "../models/Constants.js" as Constants

GeneralView {
    Column {
        anchors.fill: parent
        Item {
            // top 1/4 of screen
            width: parent.width
            height: 1/4 * parent.height

            PagedFlickable {
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width
                elementWidth: Constants.ovalButtonWidth
                snapTo: elementWidth + spacing
                spacing: (width - 4 * elementWidth) / 3

                OvalButton {
                    text: "Find New Apps"
                }

                OvalButton {
                    text: "Change device"
                    onReleased: contentLoader.go("./views/ChangeDeviceView.qml")
                }

                OvalButton {
                    text: "911 Assist"
                }

                OvalButton {
                    text: "Vehicle Health Report"
                }

                OvalButton {
                    text: "Install applications/Up"
                }
            }
        }

        Item {
            height: parent.height / 2
            width: parent.width

            ListView {
                id: applicationListView
                anchors.fill: parent
                model: dataContainer.applicationList

                anchors.horizontalCenter: parent.horizontalCenter
                width:parent.width
                height:parent.height

                delegate: Row {
                    width: parent.width
                    height: appName.height
                    Image { id: sdlIcon; source: icon }
                    Text  {
                        text: appName
                        color: "#1d81d5"
                        font.pixelSize: 40
                        anchors.verticalCenter: parent.verticalCenter
                        MouseArea {
                          anchors.fill: parent
                          onClicked: {
                              dataContainer.setCurrentApplication(appId)
                              dataContainer.currentApplication.isMediaApplication ? contentLoader.go("./views/SDLPlayerView.qml", dataContainer.currentApplication.appId)
                                                                                  : contentLoader.go("./views/SDLNonMediaView.qml", dataContainer.currentApplication.appId)
                            }
                        }
                    }
                }
            }
        }

        Item {
            // bottom 1/4 of screen
            width: parent.width
            height: 1/4 * parent.height

            BackButton { anchors.centerIn: parent }
        }
    }
}
