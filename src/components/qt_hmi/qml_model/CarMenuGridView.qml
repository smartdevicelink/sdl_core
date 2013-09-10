import QtQuick 2.0

GridView
{
    populate: Transition {
        id: populateTransition;
        SequentialAnimation {
            PropertyAction  {
                property: "opacity";
                value: 0
            }
            PauseAnimation {
                duration: populateTransition.ViewTransition.index * 100;
            }
            NumberAnimation {
                properties: "opacity"
                from: 0
                to: 1
                duration: 300
            }
        }
    }

    anchors.fill: parent
    id: carMenuGridview
    cellWidth: width / 4
    cellHeight: height / 2
    highlightFollowsCurrentItem: false
    snapMode: GridView.SnapToRow
    flickableDirection: Flickable.AutoFlickDirection
    interactive: true

    model: CarMenu {}
    highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
    focus: true

    delegate: Item {
        width: carMenuGridview.cellWidth
        height: carMenuGridview.cellHeight
        MouseArea {
            anchors.fill: parent
            onClicked: {
                caption.text = name
                mainScreen.state = name
                menuContainer.source = qml
            }
        }

        Image { source: icon; anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter;}
        Text  {
            text: name;
            color: "white"
            anchors.baseline: parent.bottom;
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Component.onCompleted: {
        console.log("abracadabra")
    }
}
