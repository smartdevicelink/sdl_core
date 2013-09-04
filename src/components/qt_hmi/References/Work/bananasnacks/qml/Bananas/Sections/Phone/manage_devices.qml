import QtQuick 2.0
import com.ford.phonecore 1.0
import "../../Components"
import "../../Layouts"

TitledLayout {
    title: "Manage Bluetooth devices"
    back_button: true

    Loader {
        id: view

        anchors.fill: parent
        source: (PhoneCore.device_name === "") ? "discovery.qml" : "device_info.qml"
    }
}
