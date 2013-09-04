import QtQuick 2.0
import "../../Components"
import "../../Layouts"

MediaPlayerLayout {
    header: "USB"
    id: usb

    Button {
        text: "Browse Media"
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 40
        anchors.topMargin: 75

        onPress: parent.state = 'browse'
    }

    states: [
        State {
            name: 'browse'
            PropertyChanges { target: browse_interface; visible: true; opacity: 0.9 }
        }

    ]

    Rectangle {
        color: "#000000"
        opacity: 0
        anchors.fill: parent
        radius: 10
        visible: false
        id: browse_interface

        BackButton {
            id: back_button
            anchors.topMargin: 80
            anchors.leftMargin: 20
        }


        H2 {
            id: header
            text: "Browse Media"
            anchors.topMargin: 80
        }

        Item {
            anchors.fill: parent
            id: browse_container
        }

        Button {
            anchors.top: back_button.bottom
            anchors.left: parent.left
            text: "Jump"
        }
    }
}
