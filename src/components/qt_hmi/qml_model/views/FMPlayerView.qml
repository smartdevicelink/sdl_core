import QtQuick 2.0
import "../controls"

GeneralView {
    RadioPlayerView {
        anchors.fill: parent

        radioType: "FM"
        presets: ["96.3", "107.9", "104.3", "101.9"]

        LongOvalButton {
            id: hdBtn
            Row {
                id: row
                anchors.verticalCenter: parent.verticalCenter
                x: parent.x + 20
                spacing: 5
                property int selected: 0
                MouseArea {
                    onClicked: {
                        //todo: change color of text
                    }
                }

                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    source:"../res/hd_logo_on.png"
                }
                Text {
                    id: one
                    anchors.verticalCenter: parent.verticalCenter
                    text: "1"
                    color: "white"
                    font.pixelSize: 25
                }
                Text {
                    id: two
                    anchors.verticalCenter: parent.verticalCenter
                    text: "2"
                    color: parent.selected == 1 ? "white" : "#1d81d5"
                    font.pixelSize: 25
                }
                Text {
                    id: three
                    anchors.verticalCenter: parent.verticalCenter
                    text: "3"
                    color: parent.selected == 2 ? "white" : "#1d81d5"
                    font.pixelSize: 25
                }
                Text {
                    id: fourth
                    anchors.verticalCenter: parent.verticalCenter
                    text: "4"
                    color: parent.selected == 3 ? "white" : "#1d81d5"
                    font.pixelSize: 25
                }
            }
        }

    }
}
