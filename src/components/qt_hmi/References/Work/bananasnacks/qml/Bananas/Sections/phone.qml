import QtQuick 2.0
import "../Components"
import "../Layouts"

Rectangle {
    id: section
    anchors.fill: parent

    states: [
        State {
            name: ''
            PropertyChanges { target: page; source: "Phone/menu.qml" }
        },
        State {
            name: 'recent_calls'
            PropertyChanges { target: page; source: "Phone/recent_calls.qml" }
        },
        State {
            name: 'keypad'
            PropertyChanges { target: page; source: "Phone/keypad.qml" }
        },
        State {
            name: 'manage_devices'
            PropertyChanges { target: page; source: "Phone/manage_devices.qml" }
        }
    ]

    gradient: Gradient {
        GradientStop { position: 0.0; color: Qt.rgba(0.454901961,0.37254902,0.203921569,1) }
        GradientStop { position: 0.4; color: "#000000" }
    }

    Section {
        id: page
        source: "Phone/menu.qml"
    }
}
