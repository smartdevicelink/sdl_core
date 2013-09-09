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

    id: appsMenuGridview
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: 3 * cellWidth
    height: 2 * cellHeight
    cellWidth: 221
    cellHeight: 149
    highlightFollowsCurrentItem: false
    snapMode: GridView.SnapToRow
    flickableDirection: Flickable.AutoFlickDirection
    interactive: true

    model: AppsMenu {}
    highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
    focus: true
    visible: parent.state === "Apps"

    delegate: Item {
        width: appsMenuGridview.cellWidth
        height: appsMenuGridview.cellHeight
        MouseArea {
            anchors.fill: parent
            onClicked: {
                caption.text = name
                mainScreen.state = name
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
}
