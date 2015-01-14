import QtQuick 2.0
import "../../Components"
import "../../Layouts"

LeftThirdSection {
    Grid {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        GraphicalButton {
            text: "Do Not Disturb"
            image: "../../Assets/DndIcon.png"
        }

        GraphicalButton {
            text: "Phonebook"
            image: "../../Assets/PhonebookIcon.png"
        }

        GraphicalButton {
            text: "Voicemail"
            image: "../../Assets/VoicemailIcon.png"
        }

        GraphicalButton {
            text: "Messages"
            image: "../../Assets/MessagesIcon.png"
        }

        GraphicalButton {
            text: "Recent Calls"
            image: "../../Assets/RecentCallsIcon.png"
            onPress: section.state = 'recent_calls'
        }

        GraphicalButton {
            text: "Keypad"
            image: "../../Assets/KeypadIcon.png"
            onPress: section.state = 'keypad'
        }

        GraphicalButton {
            text: "Speed Dial"
            image: "../../Assets/SpeedDialIcon.png"
        }

        GraphicalButton {
            text: "Devices"
            image: "../../Assets/SpeedDialIcon.png"
            onPress: section.state = 'manage_devices'
        }
    }
}
