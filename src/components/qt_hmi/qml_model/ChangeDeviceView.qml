import QtQuick 2.0

Item {
    anchors.fill: parent

    Text {

        SequentialAnimation {
            running: true;
            loops: 8
            PropertyAction { target: searchDeviceText; property: "text"; value: "Searching devices" }
            PauseAnimation { duration: 200 }
            PropertyAction { target: searchDeviceText; property: "text"; value: "Searching devices." }
            PauseAnimation { duration: 200 }
            PropertyAction { target: searchDeviceText; property: "text"; value: "Searching devices.." }
            PauseAnimation { duration: 200 }
            PropertyAction { target: searchDeviceText; property: "text"; value: "Searching devices..." }
            PauseAnimation { duration: 200 }
            onStopped: {
                searchDeviceText.text = "Search device done"
            }
        }

        //anchors.left: parent.left
        width: parent.width
        id: searchDeviceText
        color: "#1d81d5"
        font.pixelSize: 40
    }

    ListView {
        id: changeDeviceListView
        model: ChangeDeviceModel {}

        anchors.horizontalCenter: parent.horizontalCenter
        y: searchDeviceText.height
        width:parent.width
        height:parent.height - searchDeviceText.height


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
                      menuContainer.source = "ApplicationListView.qml"
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

    Text{
        id: backtext
        anchors.horizontalCenter: parent.horizontalCenter
        y: changeDeviceListView.height
        text: "Back"
        color: "#1d81d5"
        font.pixelSize: 30
        Image{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: backtext.verticalCenter
            source: "res/buttons/long_oval_btn.png"
        }
        MouseArea {
          anchors.fill: parent
          onClicked: {
              menuContainer.source = "AppsMenuGridView.qml"
            }
        }
    }
}
