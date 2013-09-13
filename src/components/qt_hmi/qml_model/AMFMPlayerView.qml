import QtQuick 2.0

Column {
    spacing: 25
    anchors.fill: parent
    property string radioType: ""
    property var activeBand: radioType === "AM"? ["1130", "950", "760", "1270"]: ["96.3", "107.9", "104.3", "101.9"]
    property var presets: [ "1130", "950", "760", "1270", "96.3", "107.9", "104.3", "101.9" ]
    Item{
        width: parent.width
        height: parent.height * 0.25
        Image{
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            source: "res/buttons/long_oval_btn.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    menuContainer.go("MusicSourceGridView.qml")
                }
            }
            Text{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "#1d81d5"
                text: radioType + " Radio"
                font.pixelSize: 20
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
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    //menuContainer.go(qml)
                }
            }
        }

    }
    Row{
        spacing: 40
        width: parent.width
        height: parent.height * 0.65
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
            Image {
                source: "res/buttons/long_oval_btn.png"
                visible: radioType === "AM" ? false: true
                Row{
                    anchors.verticalCenter: parent.verticalCenter
                    x: parent.x + 20
                    spacing: 5
                    Image {
                        id: hdImg
                        state: "on"
                        anchors.verticalCenter: parent.verticalCenter
                        source:"res/hd_logo_on.png"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(hdImg.state === "on"){
                                    hdImg.state = "off"
                                    hdImg.source = "res/hd_logo_off.png"
                                } else {
                                    hdImg.state = "on"
                                    hdImg.source = "res/hd_logo_on.png"
                                }

                            }
                        }
                    }
                    Text {
                        text: "1"
                        color: "white"
                        font.pixelSize: 25
                    }
                    Text {
                        text: "2"
                        color: "#1d81d5"
                        font.pixelSize: 25
                    }
                    Text {
                        text: "3"
                        color: "#1d81d5"
                        font.pixelSize: 25
                    }
                    Text {
                        text: "4"
                        color: "#1d81d5"
                        font.pixelSize: 25
                    }
                }
            }
        }
    }
    Rectangle{
        width: parent.width
        height: 2
        color: "#1d81d5"
    }

//*
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

//*/
/*
    Row {
        id: presetRow
        width: parent.width
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
                text: activeBand[0]
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
                text: activeBand[1]
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
                text: activeBand[2]
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
                text: activeBand[3]
                font.pixelSize: 25
                color: "white"
            }
        }
    }
//*/


}
