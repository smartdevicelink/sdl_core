import QtQuick 2.0
import com.ford.phonecore 1.0
import "../Components"
import "../Layouts"

Item {

    Image {
        source: "../Assets/Crosshairs.png"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    Item {
        width: parent.width / 2
        height: parent.height / 2

        Image {
            id: phone_corner_icon
            visible: (PhoneCore.device_state === "connected")
            source: "../Assets/PhoneIcons.png"
            anchors.top: parent.top
            anchors.topMargin: 80
            anchors.horizontalCenter: parent.horizontalCenter
        }

        OverflowableText {
            id: phone_name
            text: PhoneCore.device_name
            anchors.top: phone_corner_icon.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            textAlign: 'center'
            anchors.topMargin: 10
            width: 350
            fontColor: (PhoneCore.device_state === "connected") ? "#ffffff" : "#333"
            font.pixelSize: 24
        }

        Text {
            text: "Connecting..."
            anchors.top: phone_name.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
            color: "#333"
            visible: (PhoneCore.device_state === "connecting")
            font.pixelSize: 24
        }

        CheckButton {
            text: "Do Not Disturb"
            visible: (PhoneCore.device_state === "connected")
            anchors.top: phone_name.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
        }



        Row {
            anchors.top: phone_name.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
            visible: (PhoneCore.device_state === "disconnected")
            spacing: 10

            Button {
                text: "Connect"
                onPress: {
                    PhoneCore.initializeDevice()
                }
            }

            Button {
                text: "Manage Phones"
                onPress: {
                    go("phone");
                }
            }
        }

    }

    Item {
        width: parent.width / 2
        height: parent.height / 2
        anchors.right: parent.right
    }

    Item {
        width: parent.width / 2
        height: parent.height / 2
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Image {
            source: "../Assets/VehicleCornerIcon.png"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Item {
        width: parent.width / 2
        height: parent.height / 2
        anchors.bottom: parent.bottom

        Image {
            id: media_corner_icon
            source: "../Assets/UsbIcon.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 40
        }

        Text {
            id: media_artist
            text: ""
            font.pixelSize: 18
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: media_corner_icon.bottom
            color: "#ffffff"
            anchors.topMargin: 10
        }

        Text {
            id: media_title
            text: ""
            font.pixelSize: 24
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: media_artist.bottom
            color: "#ffffff"
        }
    }
}
