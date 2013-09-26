import QtQuick 2.0
import "../controls"

Column {
    id: radioPlayer
    anchors.fill: parent
    property string radioType: ""
    property var activeBand: radioType === "AM"? ["1130", "950", "760", "1270"]: ["96.3", "107.9", "104.3", "101.9"]
    property var presets: [ "1130", "950", "760", "1270", "96.3", "107.9", "104.3", "101.9" ]
    property int minHeight: 400
    Item{
        id: upperControlLine
        width: parent.width
        height: parent.height / 4
        anchors.horizontalCenter: parent.horizontalCenter
        LongOvalButton {
            text: radioType + " Radio"
            pixelSize: 20
            dest: "./views/MusicSourceGridView.qml"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }

        LongOvalButton {
            text: "Tune"
            pixelSize: 20
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    Item{
        width: parent.width
        height: parent.height / 2

        Column {
            spacing: 10
            Row{
                Text{
                    id: radioChannelNameText
                    color: "#1d81d5"
                    text: presets[0]
                    font.pixelSize: 45
                }
                Text{
                    color: "#1d81d5"
                    text: " " + radioType
                    font.pixelSize: 25
                    anchors.bottom: radioChannelNameText.bottom
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
            LongOvalButton {
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

    Item {
        width: parent.width
        height: parent.height / 4
        Rectangle{
            width: parent.width
            height: 2
            color: "#1d81d5"
        }
        PresetRow{
            anchors.centerIn: parent
            presets: radioPlayer.presets
            width: parent.width
            onSelectedIndexChanged: {
                radioChannelNameText.text = presets[selectedIndex];
            }
        }
    }

}
