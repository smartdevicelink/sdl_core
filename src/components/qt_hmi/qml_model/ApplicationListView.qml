import QtQuick 2.0

Item {
    anchors.fill: parent

    ListView {
        id: applicationListView
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
            MouseArea {
              anchors.fill: parent
              onClicked: {
                  menuContainer.source = "SDLPlayerView.qml"
                }
            }
            Image { id:sdlIcon;source: icon}
            Text  {
                x: sdlIcon.width
                text: name
                color: "#1d81d5"
                font.pixelSize: 40
                //anchors.baseline: parent.bottom;
                //anchors.horizontalCenter: parent.horizontalCenter
            }
        }

    }

    Text{
        id: backtext
        anchors.horizontalCenter: parent.horizontalCenter
        y: applicationListView.height
        text: "Back"
        color: "#1d81d5"
        font.pixelSize: 30
        MouseArea {
          anchors.fill: parent
          onClicked: {
              menuContainer.source = "ChangeDeviceView.qml"
            }
        }
    }
}
