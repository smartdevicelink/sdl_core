import QtQuick 2.0
import "../controls"

Column {
    id: siriusPlayer
    spacing: 25
    anchors.fill: parent
    property var presets: [ "Lithium", "Spectrum", "ESPN", "Alt Nation", "Lithium", "Spectrum", "ESPN", "Alt Nation" ]
    Item{
        width: parent.width
        height: parent.height / 4
        LongOvalButton {
            text: "Sirius"
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
    Row {
        spacing: 50
        width: parent.width
        x: 35
        Column{
            spacing: 10
            Row{
                Text{
                    id: siriusChannelNameText
                    color: "#1d81d5"
                    text: presets[dataContainer.playerPreferences.Sirius.preset]
                    font.pixelSize: 45
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
        PresetRow {
            anchors.centerIn: parent
            presets: siriusPlayer.presets
            width: parent.width - 100
            onSelectedIndexChanged: {
                siriusChannelNameText.text = presets[selectedIndex];
            }
        }
    }
}
