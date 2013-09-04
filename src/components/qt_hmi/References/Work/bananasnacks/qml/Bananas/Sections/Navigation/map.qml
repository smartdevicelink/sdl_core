import QtQuick 2.0
import "../../Components"

Item {
    Column {
        anchors.left: parent.left;
        anchors.verticalCenter: parent.verticalCenter
        spacing: 10

        Button {
            text: "Zoom in"
        }

        Button {
            text: "Zoom out"
        }

        Button {
            text: "3D"
        }

        Button {
            text: "2D"
        }

        Button {
            text: "North up"
        }
    }

    Rectangle {
        width: next_turn.width + 20
        height: 30
        anchors.top: parent.top
        anchors.topMargin: 65
        anchors.horizontalCenter: parent.horizontalCenter
        color: "black"
        opacity: 0.6

    }

    Rectangle {
        width: current_street.width + 20
        height: 30
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 65
        anchors.horizontalCenter: parent.horizontalCenter
        color: "black"
        opacity: 0.6
        visible: navController.current_street.length > 0

    }


    Button {
        text: "Go"
        anchors.top: parent.top
        anchors.topMargin: 3
        anchors.right: parent.right
        anchors.rightMargin: 20
        onPress: section.state = 'menu'
    }
}
