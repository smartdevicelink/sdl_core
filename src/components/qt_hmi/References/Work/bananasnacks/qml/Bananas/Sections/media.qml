import QtQuick 2.0
import "../Components"
import "../Layouts"

Rectangle {
    anchors.fill: parent
    id: section

    gradient: Gradient {
        GradientStop { position: 0.8; color: "#000000" }
        GradientStop { position: 1.0; color: "#7C3232" }
    }

    states: [
        State {
            name: 'menu'
            PropertyChanges { target: page; source: "Media/menu.qml" }
        },
        State {
            name: 'fm'
            PropertyChanges { target: page; source: "Media/fm.qml" }
        },
        State {
            name: 'sirius'
            PropertyChanges { target: page; source: "Media/sirius.qml" }
        },
        State {
            name: 'usb'
            PropertyChanges { target: page; source: "Media/usb.qml" }
        },
        State {
            name: 'bluetooth'
            PropertyChanges { target: page; source: "Media/bluetooth.qml" }
        },
        State {
            name: 'applink'
            PropertyChanges { target: page; source: "Media/applink.qml" }
        }
    ]

    transitions: [
        Transition {
            to: "bluetooth"
        },
        Transition {
            to: "usb"
        }

    ]

    Section {
        id: page
        source: "Media/usb.qml"
    }
}
