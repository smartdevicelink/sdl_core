import QtQuick 2.0
import "../controls"

Item {
    id: radioPlayer

    property string radioType: ""
    property var activeBand: radioType === "AM"? ["1130", "950", "760", "1270"]: ["96.3", "107.9", "104.3", "101.9"]
    property var presets: [ "1130", "950", "760", "1270", "96.3", "107.9", "104.3", "101.9" ]
    property int minHeight: 400

    default property alias content: additional.children

    Column {
        anchors.fill: parent

        Item {
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
        Item {
            width: parent.width
            height: parent.height / 2

            Column {
                spacing: 10
                Row {
                    Text {
                        id: radioChannelNameText
                        color: "#1d81d5"
                        text: presets[0]
                        font.pixelSize: 45
                    }
                    Text {
                        color: "#1d81d5"
                        text: " " + radioType
                        font.pixelSize: 25
                        anchors.bottom: radioChannelNameText.bottom
                    }
                }
                Row {
                    Text {
                        color: "#1d81d5"
                        text: "Song name"
                        font.pixelSize: 25
                        font.bold: true
                    }
                }
                Row {
                    Text {
                        color: "#1d81d5"
                        text: "Album Name"
                        font.pixelSize: 25

                    }
                }
                Item {
                    width: childrenRect.width
                    height: childrenRect.height
                    id: additional
                }
            }
        }

        Item {
            width: parent.width
            height: parent.height / 4
            Rectangle {
                width: parent.width
                height: 2
                color: "#1d81d5"
            }
            PresetRow {
                anchors.centerIn: parent
                presets: radioPlayer.presets
                width: parent.width
                onSelectedIndexChanged: {
                    radioChannelNameText.text = presets[selectedIndex];
                }
            }
        }
    }
}
