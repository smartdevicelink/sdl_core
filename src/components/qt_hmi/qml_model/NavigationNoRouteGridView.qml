import QtQuick 2.0

GridView{
        id: navigationNoRouteGridView


    anchors.fill: parent

    Image {
        anchors.centerIn: parent
        source: "res/nav/map.png"
    }

    Image{
        id: zoomOut
        source: "res/buttons/zoom.png"
        x:0
        y:120

        Text{
            anchors.centerIn: parent
            text: "-"
            color: "white"
            font.pixelSize: 30
        }
    }

    Image{
        id: zoomIn
        source: "res/buttons/zoom.png"
        x:0
        y:10

        Text{
            anchors.centerIn: parent
            text: "+"
            color: "white"
            font.pixelSize: 30
        }
    }

    Image{
        id: compas
        source: "res/nav/compass.png"
        x:10
        y:250
    }

    Image{
        id: options
        source: "res/nav/options.png"
        x:640
        y:0
    }

    Image{
        id: destSearch
        source: "res/buttons/longest_oval_btn.png"
        x:600
        y:320

        Image{
            anchors.verticalCenter: parent.verticalCenter
            x:15
            source: "res/nav/destIcon.png"
        }
        Text{
            anchors.verticalCenter: parent.verticalCenter
            x:40
            text: "Destination"
            color: "#1d81d5"
            font.pixelSize: 20
        }

        MouseArea {
            cursorShape: Qt.PointingHandCursor
            anchors.fill: parent
            onClicked: {
                menuContainer.go("NavigationMenuGridView.qml")
            }
        }
    }




}
