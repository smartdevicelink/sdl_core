import QtQuick 2.0
import com.ford.sdlcore 1.0
import "../../Components"
import "../../Layouts"

LeftThirdSection {
    header: "Select a source"

    Grid {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        GraphicalButton {
            text: "Radio"
            image: "../../Assets/RadioIcon.png"
            onPress: section.state = 'fm'
        }

        GraphicalButton {
            text: "Pandora"
            image: "../../Assets/PandoraIcon.png"
            onPress: section.state = 'fm'
        }

        GraphicalButton {
            text: "Sirius"
            image: "../../Assets/SiriusIcon.png"
            onPress: section.state = 'sirius'
        }

        GraphicalButton {
            text: "Bluetooth"
            image: "../../Assets/BluetoothIcon.png"
            onPress: section.state = 'bluetooth'
        }

        GraphicalButton {
            text: "USB"
            image: "../../Assets/UsbIcon.png"
            onPress: section.state = 'usb'
        }

        Repeater {
            model: MediaApps

            delegate: GraphicalButton {
                text: modelData
                image: "../../Assets/UsbIcon.png"
                onPress: section.state = 'applink'
            }
        }
    }
}
