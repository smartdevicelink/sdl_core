import QtQuick 2.0
import "Components"
import com.ford.sdlcore 1.0
import com.ford.phonecore 1.0

Rectangle {
    id: root
    width: 800
    height: 470
    color: "#000"

    states: [
        State {
            name: "home"
            PropertyChanges { target: maincontent; source: "Sections/home.qml" }
        },
        State {
            name: "home_corners"
            PropertyChanges { target: maincontent; source: "Sections/home_corners.qml" }
        },
        State {
            name: "phone"
            PropertyChanges { target: maincontent; source: "Sections/phone.qml" }
        },
        State {
            name: "navigation"
            PropertyChanges { target: maincontent; source: "Sections/navigation.qml" }
        },
        State {
            name: "media"
            PropertyChanges { target: maincontent; source: "Sections/media.qml" }
        },
        State {
            name: "vehicle"
            PropertyChanges { target: maincontent; source: "Sections/vehicle.qml" }
        }
    ]

    transitions: [
        Transition {
            to: '*'
            PropertyAnimation { target: maincontent; property: 'opacity'; from: 0; to: 1}
        }
    ]

    function go(string) {
        state = string
    }

    Connections {
        target: AppLink
        onAlert: root.alert(message);
    }

    Connections {
        target: PhoneCore
        onCallAdded: root.alert("Incoming call: \n" + name + "\n" + number)
        onCallRemoved: root.alert("Call ended")
    }

    function alert(message) {
        alert_loader.item.alert(message)
    }

    Section {
        id: maincontent
        source: "Sections/home.qml"
    }

    Section {
        id: alert_loader
        source: "Sections/alert.qml"
    }

    Section {
        id: chrome
        source: "Sections/chrome.qml"
    }
}
