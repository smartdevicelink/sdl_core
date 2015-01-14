import QtQuick 2.0
import "../Components"
import "../Layouts"
import com.ford.phonecore 1.0

LeftThirdSection {
    header: "Good Morning, Jen"

    Column {
        anchors.left: parent.left
        anchors.leftMargin: 60
        spacing: 40
        anchors.verticalCenter: parent.verticalCenter

        Weather {}
        Weather {
            text: "Tonight"
            temp: "55"
            image: "../Assets/CloudNight.png"
        }
    }


    Image {
        id: schedule
        source: "../Assets/CMaxCal.png"
        anchors.right: parent.right
        anchors.rightMargin: 70
        anchors.verticalCenter: parent.verticalCenter
    }

    Column {
        anchors.right: schedule.left
        anchors.rightMargin: 20
        anchors.top: schedule.top
        anchors.topMargin: 5
        spacing: 40

        Button {
            text: "Dial"
        }
        Button {
            text: "Navigate"
        }
        Button {
            text: "Siri"
            onPress: PhoneCore.siriEyesFree()
            visible: PhoneCore.device_state === "connected" || PhoneCore.device_state === "playing"
        }
    }
}
