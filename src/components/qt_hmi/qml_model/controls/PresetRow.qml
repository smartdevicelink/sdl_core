import QtQuick 2.0
import com.ford.hmi_framework 1.0

Item {
    width: parent.width
    height: 100
    Rectangle{
        width: parent.width
        height: 2
        color: "#1d81d5"
    }

    Flickable
    {
        anchors.fill: parent
        contentHeight: 100
        contentWidth: presetRow.width
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        Grid {
            id: presetRow

            columns: presets.length
            rows: 1
            spacing: 160

            Repeater {
                model : presets.length

                Image {
                    source: presetRow.state == index.toString() ? "../res/buttons/preset_pressed_btn.png" : "../res/buttons/preset_btn.png"
                    //anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            siriusChannelNameText.text = presets[index]
                            presetRow.state = index.toString()
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
                        text: presets[index]
                        font.pixelSize: 25
                        color: "white"
                    }
                }
            }
        }
    }
    Item {
        width: 1
        height: 20
        anchors.horizontalCenter: parent.horizontalCenter
        Image {
            source: "../res/white_ball.png"
        }
        Image {
            x: 15
            source: "../res/blue_ball.png"
        }
    }

}
