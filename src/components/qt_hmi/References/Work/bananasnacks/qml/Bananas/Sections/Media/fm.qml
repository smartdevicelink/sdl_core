import QtQuick 2.0
import "../../Components"
import "../../Layouts"

MediaLayout {
    header: "FM"

    Flickable {
        width: 700
        height: 50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 80
        contentHeight: height
        contentWidth: row.width
        clip: true

        RadioButtonGroup {
            width: row.width
            height: row.height
            Row {
                id: row
                spacing: 8

                RadioButton {
                    text: "Button"
                }
                RadioButton {
                    text: "Button"
                }
                RadioButton {
                    text: "Button"
                }
                RadioButton {
                    text: "Button"
                }
                RadioButton {
                    text: "Button"
                }
                RadioButton {
                    text: "Button"
                }
            }
        }
    }
}
