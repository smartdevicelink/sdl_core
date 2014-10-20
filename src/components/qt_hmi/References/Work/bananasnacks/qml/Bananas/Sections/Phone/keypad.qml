import QtQuick 2.0
import com.ford.phonecore 1.0
import "../../Components"
import "../../Layouts"

Item {
    anchors.top: parent.top
    anchors.topMargin: 80

    TextBox {
        id: phone_number
        border.color: Qt.rgba(0.454901961,0.37254902,0.203921569,1)
        anchors.top: parent.top
        width: parent.width - 160
        anchors.horizontalCenter: parent.horizontalCenter
    }

    BackButton {
        anchors.top: phone_number.top
        anchors.topMargin: 1
        anchors.right: phone_number.left
        anchors.rightMargin: 10
        onPress: section.state = ''
    }

    ImageButton {
        anchors.top: phone_number.top
        anchors.topMargin: 1
        icon: "../../Assets/DeleteIcon.png"
        anchors.left: phone_number.right
        anchors.leftMargin: 10
        height: 42

        onPress: phone_number.text = phone_number.text.replace(/.$/, '')
    }

    Keypad {
        anchors.topMargin: 40
        anchors.top: phone_number.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        onLetterClicked: phone_number.text += letter
    }

    Button {
        text: "Call"
        onPress: PhoneCore.dial(phone_number.text);
        y: 100
    }
    Button {
        text: "End call"
        onPress: PhoneCore.hangup();
        y: 160
    }
}
