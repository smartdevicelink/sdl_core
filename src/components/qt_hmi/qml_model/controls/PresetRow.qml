import QtQuick 2.0

Item {
    id: presetRow
    width: parent.width
    height: childrenRect.height

    property variant presets: [];
    property int selectedIndex: 0
    signal presetSelected

    Column {
        anchors.fill: parent

        PagedFlickable {
            width: parent.width
            height: 200
            snapTo: spacing + 63
            spacing: (width - (63 * 4)) / 3

            Repeater {
                model: presetRow.presets.length
                delegate: Image {
                    source: presetRow.selectedIndex === index ? "../res/buttons/preset_pressed_btn.png" : "../res/buttons/preset_btn.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            presetRow.selectedIndex = index
                        }
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: index + 1
                        font.pixelSize: 30
                        color: "#1d81d5"
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: parent.width
                        text: presetRow.presets[index]
                        font.pixelSize: 25
                        color: "white"
                    }
                }
            }
        }
    }
}
