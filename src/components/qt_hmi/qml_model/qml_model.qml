import QtQuick 2.0

Rectangle {
    id: rectangle1
    width: 1360
    height: 768
    color: "black"

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }

        Grid {
            id: upperMenuGrid
            x: mainMenuGridview.x - 50
            y: mainMenuGridview.y - 80
            width: mainMenuGridview.width + 100
            height: 80
            Text{
                text: "75°";
                color: "#1d81d5"
                font.pixelSize: 25;
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            Text{
                text: "MENU";
                color: "#1d81d5"
                font.pixelSize: 25;
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            Text{
                text: "12:50";
                color: "#1d81d5"
                font.pixelSize: 25;
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    GridView
    {
        id: mainMenuGridview
        x: 334
        y: 259
        width: 667
        height: 303
        cellWidth: 221
        cellHeight: 149
        highlightFollowsCurrentItem: false
        snapMode: GridView.SnapToRow
        flickableDirection: Flickable.AutoFlickDirection
        interactive: true


        model: Menu {}
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true

        delegate: Item {
            width: mainMenuGridview.cellWidth
            height: mainMenuGridview.cellHeight
            Image { source: icon; anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter;}
            /*Text  {
                text: name;
                color: "white"
                anchors.baseline: parent.bottom;
                anchors.horizontalCenter: parent.horizontalCenter
            }*/
        }
    }
}

