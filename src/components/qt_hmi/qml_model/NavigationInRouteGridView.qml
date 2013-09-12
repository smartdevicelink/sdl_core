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
        source: "res/buttons/long_oval_btn.png"
        x:600
        y:320

        Image{
            anchors.verticalCenter: parent.verticalCenter
            x:20
            source: "res/nav/destIcon.png"
        }
        Text{
            anchors.centerIn: parent
            text: "Cancel"
            color: "#1d81d5"
            font.pixelSize: 20
        }

        MouseArea {
            cursorShape: Qt.PointingHandCursor
            anchors.fill: parent
            onClicked: {
                menuContainer.go("NavigationNoRouteGridView.qml")
            }
        }
    }

    Image{
        id: currentStreet
        source: "res/nav/current_street.png"
        anchors.horizontalCenter: parent.horizontalCenter
        y:250

        Text{
            anchors.centerIn: parent
            text: "East Capital St. NE"
            color: "White"
            font.pixelSize: 18
        }

    }

    Image{
        x:0
        y:320
        id: mute
        source: "res/buttons/mute.png"

    }

    Image{
        x:85
        y:339.5
        source: "res/nav/turnArrow.png"
    }

    Text{
        x:130
        y:339.5
        text: "0.2 mi on Cherry Hill Rd."
        color: "White"
        font.pixelSize: 20
    }

    Text{
        x:450
        y:339.5
        text: "2 hrs 27 min"
        color: "White"
        font.pixelSize: 20
    }











}
