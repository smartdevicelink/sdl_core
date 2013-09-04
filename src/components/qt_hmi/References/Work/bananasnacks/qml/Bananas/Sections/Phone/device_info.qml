import QtQuick 2.0
import "../../Components"
import com.ford.phonecore 1.0

Item {
    anchors.fill: parent

    H2 {
        id: h2
        text: PhoneCore.device_name

        anchors.horizontalCenter: parent.horizontalCenter
    }
    Button {
        text: "Forget device"
        onPress: PhoneCore.unpairDevice()
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: h2.bottom
        anchors.topMargin: 20
    }
}
