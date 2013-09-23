import QtQuick 2.0
import "../controls"
import "../models"

Item {
    anchors.fill: parent

    Item {
        // top 3/4 of screen
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left

        ListView {
            id: applicationListView
            anchors.fill: parent
            model: ApplicationListModel {}

            anchors.horizontalCenter: parent.horizontalCenter
            //y: searchDeviceText.height
            width:parent.width
            height:parent.height


            //highlightFollowsCurrentItem: false
            //flickableDirection: Flickable.AutoFlickDirection
            //interactive: true

            //highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            //focus: true


            delegate: Item{
                width: parent.width
                height: sdlIcon.height
                Image { id:sdlIcon;source: icon}
                Text  {
                    x: sdlIcon.width
                    text: name
                    color: "#1d81d5"
                    font.pixelSize: 40
                    MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          contentLoader.go("./views/SDLPlayerView.qml")
                        }
                    }
                }
            }
        }
    }

    Item{
        // bottom 1/4 of screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        BackButton{}
    }
}
