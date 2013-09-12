import QtQuick 2.0

Column {
    spacing: 25
    anchors.fill: parent
    Item{
        width: parent.width
        height: parent.height / 4
        Image{
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            source: "res/buttons/long_oval_btn.png"
            Text{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "#1d81d5"
                text: "Sirius"
                font.pixelSize: 20
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        menuContainer.go("MusicSourceGridView.qml")
                    }
                }
            }
        }

        Image{
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            source: "res/buttons/long_oval_btn.png"
            Text{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "#1d81d5"
                text: "Tune"
                font.pixelSize: 20
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        //menuContainer.go(qml)
                    }
                }
            }
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

}
