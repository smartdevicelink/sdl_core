import QtQuick 2.0
import "./controls"

Column {
    id: climateControlBtn
    spacing: 25
    anchors.fill: parent
    property var presets: [ "Lithium", "Spectrum", "ESPN", "Alt Nation", "Lithium", "Spectrum", "ESPN", "Alt Nation" ]
    Item{
        width: parent.width
        height: parent.height / 4
        LongOvalBtn {
            text: "Sirius"
            pixelSize: 20
            dest: "MusicSourceGridView.qml"
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
        spacing: 50
        width: parent.width
        x: 35
        Column{
            spacing: 10
            Row{
                Text{
                    id: siriusChannelNameText
                    color: "#1d81d5"
                    text: firstPresetNameText.text
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
    Row{
        width: parent.width
        height: parent.height / 4
        Rectangle{
            y: parent.height
            width: parent.width
            height: 2
            color: "#1d81d5"
        }
    }

    Item {
        width: parent.width
        height: 100
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
                        source: presetRow.state == index.toString() ? "res/buttons/preset_pressed_btn.png" : "res/buttons/preset_btn.png"
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
                source: "res/white_ball.png"
            }
            Image {
                x: 15
                source: "res/blue_ball.png"
            }
        }

    }
/*
    Row{
        id: presetRow
        width: parent.width
        height: parent.height / 4
        spacing: width / 7 + 10

        Image {
            id: firstPresetImg
            source: "res/buttons/preset_btn.png"
            anchors.verticalCenter: parent.verticalCenter
            state: "on"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    siriusChannelNameText.text = firstPresetNameText.text
                }
            }
            Text {
                id: firstPresetNumberText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: "1"
                font.pixelSize: 30
                color: "#1d81d5"
            }
            Text {
                id: firstPresetNameText
                anchors.horizontalCenter: parent.horizontalCenter
                y: parent.width
                text: "Lithium"
                font.pixelSize: 25
                color: "white"
            }
        }
        Image {
            id: secondPresetImg
            source: "res/buttons/preset_btn.png"
            anchors.verticalCenter: parent.verticalCenter
            state: "off"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    siriusChannelNameText.text = secondPresetNameText.text
                }
            }
            Text {
                id: secondPresetNumberText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: "2"
                font.pixelSize: 30
                color: "#1d81d5"
            }
            Text {
                id: secondPresetNameText
                anchors.horizontalCenter: parent.horizontalCenter
                y: parent.width
                text: "Spectrum"
                font.pixelSize: 25
                color: "white"
            }
        }
        Item {
            width: 1
            height: 20
            Image {
                source: "res/white_ball.png"
            }
            Image {
                x: 15
                source: "res/blue_ball.png"
            }
        }

        Image {
            id: thirdPresetImg
            source: "res/buttons/preset_btn.png"
            anchors.verticalCenter: parent.verticalCenter
            state: "off"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    siriusChannelNameText.text = thirdPresetNameText.text
                }
            }
            Text {
                id: thirdPresetNumberText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: "3"
                font.pixelSize: 30
                color: "#1d81d5"
            }
            Text {
                id: thirdPresetNameText
                anchors.horizontalCenter: parent.horizontalCenter
                y: parent.width
                text: "ESPN"
                font.pixelSize: 25
                color: "white"
            }
        }

        Image {
            id: fourthPresetImg
            source: "res/buttons/preset_btn.png"
            anchors.verticalCenter: parent.verticalCenter
            state: "off"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    siriusChannelNameText.text = fourthPresetNameText.text
                }
            }
            Text {
                id: fourthPresetNumberText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: "4"
                font.pixelSize: 30
                color: "#1d81d5"
            }
            Text {
                id: fourthPresetNameText
                anchors.horizontalCenter: parent.horizontalCenter
                y: parent.width
                text: "Alt Nation"
                font.pixelSize: 25
                color: "white"
            }
        }

    }
//*/

}
