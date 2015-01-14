import QtQuick 2.0
import com.ford.phonecore 1.0
import "../../Components"
import "../../Layouts"

TitledLayout {
    title: "Recent calls"
    Component.onCompleted: PhoneCore.hangup()
    back_button: true

    Flickable {
        height: 300
        width: 400
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        clip: true
        contentWidth: content.width
        contentHeight: content.height

        Column {
            id: content
            width: parent.width
            spacing: 10

            Button {
                text: "Chris Church: Mobile"
                onPress: PhoneCore.dial("8106567195")
                width: parent.width
            }
            Button {
                text: "Barry Church: Mobile"
                onPress: PhoneCore.dial("3134074522")
                width: parent.width
            }
            Button {
                text: "Barry Church: Work"
                onPress: PhoneCore.dial("3134074522")
                width: parent.width
            }
            Button {
                text: "Aaron Church: Mobile"
                onPress: PhoneCore.dial("3134074522")
                width: parent.width
            }
            Button {
                text: "Aaron Church: Mobile"
                onPress: PhoneCore.dial("3134074522")
                width: parent.width
            }
            Button {
                text: "Aaron Church: Mobile"
                onPress: PhoneCore.dial("3134074522")
                width: parent.width
            }
            Button {
                text: "Aaron Church: Mobile"
                onPress: PhoneCore.dial("3134074522")
                width: parent.width
            }
        }
    }
}
