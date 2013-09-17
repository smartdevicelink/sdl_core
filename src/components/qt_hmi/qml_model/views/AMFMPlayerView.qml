import QtQuick 2.0
import "../controls"

Column {
    spacing: 25
    anchors.fill: parent
    property string radioType: ""
    property var activeBand: radioType === "AM"? ["1130", "950", "760", "1270"]: ["96.3", "107.9", "104.3", "101.9"]
    property var presets: [ "1130", "950", "760", "1270", "96.3", "107.9", "104.3", "101.9" ]
    Item{
        width: parent.width
        height: parent.height * 0.25
        anchors.horizontalCenter: parent.horizontalCenter
        LongOvalBtn {
            text: radioType + " Radio"
            pixelSize: 20
            dest: "./views/MusicSourceGridView.qml"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }

        LongOvalBtn {
            text: "Tune"
            pixelSize: 20
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    Row{
        spacing: 40
        width: parent.width
        height: parent.height * 0.5
        x: 10
        Column {
            spacing: 10
            Row{
                Text{
                    id: siriusChannelNameText
                    color: "#1d81d5"
                    text: presets[0]
                    font.pixelSize: 45
                }
                Text{
                    color: "#1d81d5"
                    text: " " + radioType
                    font.pixelSize: 25
                    anchors.bottom: siriusChannelNameText.bottom
                }
            }
            Row {
                Text{
                    color: "#1d81d5"
                    text: "Song name"
                    font.pixelSize: 25
                    font.bold: true
                }
            }
            Row {
                Text{
                    color: "#1d81d5"
                    text: "Album Name"
                    font.pixelSize: 25

                }
            }
            LongOvalBtn {
                id: hdBtn
                visible: radioType === "AM" ? false: true
                Row{
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
    PresetRow{
        height: parent.height * 0.25
    }

}
