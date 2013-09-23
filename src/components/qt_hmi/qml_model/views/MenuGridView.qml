import QtQuick 2.0


GridView
{
    id: menuGridView

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

    property int columns: 4
    property int rows: 2

    cellWidth: width / columns
    cellHeight: height / rows

    highlightFollowsCurrentItem: false
    snapMode: GridView.SnapToRow
    flickableDirection: Flickable.AutoFlickDirection
    interactive: true

    delegate: Item {
        width: menuGridView.cellWidth
        height: menuGridView.cellHeight
        MouseArea {
            cursorShape: Qt.PointingHandCursor
            anchors.fill: parent
            onClicked: {
                if(qml !== "") {
                    contentLoader.go(qml)
                }
            }
        }

        Image { source: icon; anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter;}
    }
}
