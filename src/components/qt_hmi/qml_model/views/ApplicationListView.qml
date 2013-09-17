import QtQuick 2.0
import "../controls"
import "../models"

Item {
    anchors.fill: parent

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

    BackBtn{y: applicationListView.height}
}
