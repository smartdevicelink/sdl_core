import QtQuick 2.0

Item {
    anchors.fill: parent
    Text{
        //anchors.left: parent.left
        width: parent.width
        id: searchDeviceText
        text: "Search Devices"
        color: "#1d81d5"
        font.pixelSize: 40
    }

    ListView {
        id: changeDeviceListView
        model: ChangeDeviceModel {}

        anchors.horizontalCenter: parent.horizontalCenter
        y: searchDeviceText.height
        width:parent.width
        height:parent.height //- searchDeviceText.height

        //highlightFollowsCurrentItem: false
        //flickableDirection: Flickable.AutoFlickDirection
        //interactive: true

        //highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        //focus: true


        delegate:
            Text  {
            //Image { source: icon;}
                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                        caption.text = name
                        mainScreen.state = name
                        menuContainer.source = qml
                    }
                }
                text: name
                color: "#1d81d5"
                font.pixelSize: 50
                //anchors.baseline: parent.bottom;
                //anchors.horizontalCenter: parent.horizontalCenter
            }

        populate:  Transition {
            id: populateTransition;
            SequentialAnimation {
                PropertyAction  {
                    property: "opacity";
                    value: 0
                }
                PauseAnimation {
                    duration: populateTransition.ViewTransition.index * 2000 + 1000;
                }
                NumberAnimation {
                    properties: "opacity"
                    from: 0
                    to: 1
                    duration: 300
                }
            }
        }
    }
}
