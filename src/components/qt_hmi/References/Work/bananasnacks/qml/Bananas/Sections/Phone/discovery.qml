import QtQuick 2.0
import com.ford.phonecore 1.0
import "../../Components"

Item {
    ListView {
        anchors.horizontalCenter: parent.horizontalCenter
        width: 300
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        spacing: 10

        delegate: Button {
            text: name
            width: parent.width
            onPress: bluetooth_devices.createPairedDevice(mac)
        }

        model: BluetoothDiscoveredDevices {
            id: bluetooth_devices
        }
    }
}
