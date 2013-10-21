import QtQuick 2.0
import "../controls"
import "../models"

GeneralView {
    Column {
        anchors.fill: parent
        Item {
            // top 1/4 of screen
            width: parent.width
            height: 1/4 * parent.height

            PagedFlickable {
                width: parent.width
                snapTo: 200 + spacing
                spacing: (width - 600) / 2
                LongOvalButton {
                    width: 200
                    text: "Vehicle Health Report"
                }
                LongOvalButton {
                    width: 200
                    text: "911 Assist"
                }
                LongOvalButton {
                    width: 200
                    text: "Install applications/Up"
                }
                LongOvalButton {
                    width: 200
                    text: "Find New Apps"
                }
                LongOvalButton {
                    width: 200
                    text: "Change device"
                    dest: "./views/ChangeDeviceView.qml"
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

            BackButton { anchors.centerIn: parent}
        }
    }
}
