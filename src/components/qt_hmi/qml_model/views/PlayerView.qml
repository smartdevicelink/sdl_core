import QtQuick 2.0

import QtQuick 2.0
import "../controls"

Item {
    id: playerView
    anchors.fill: parent
    property string playerType: ""

    Item {
        // 3/4 top screen
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Rectangle {
            anchors.fill: parent
            color: "lightgreen"
        }

        LongOvalButton {
            text: playerType
            pixelSize: 20
            dest: "./views/MusicSourceGridView.qml"
            anchors.left: parent.left
            anchors.top: parent.top
        }

        LongOvalButton {
            text: "Browse"
            pixelSize: 20
            anchors.right: parent.right
            anchors.top: parent.top
        }

        Row {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 1/20 * parent.width
            spacing: albumArtImage.width * 1/3

            Image {
                id: albumArtImage
                source: "../res/album_art.png"
            }

            Column {
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    color: "#1d81d5"
                    text: "Track 13 / 16"
                    font.pixelSize: 20
                }

                Text {
                    color: "#1d81d5"
                    text: "Song name"
                    font.pixelSize: 45
                    font.bold: true
                }

                Text {
                    color: "#1d81d5"
                    text: "Album Name"
                    font.pixelSize: 25
                }
            }
        }

        Item {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: parent.height * 1/10

            Text {
                id: time
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                text: "02:36"
                font.pixelSize: 18
            }

            Rectangle {
                id: firstRect
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: time.right
                anchors.leftMargin: parent.width * 1/50
                height: 2
                width: 1/5 * parent.width
                color: "white"
            }

            Rectangle {
                id: secondRect
                anchors.left: firstRect.right
                anchors.verticalCenter: parent.verticalCenter
                height: 2
                width: 2/5 * parent.width
                color: "#1d81d5"
            }

            Text {
                anchors.left: secondRect.right
                anchors.leftMargin: parent.width * 1/50
                anchors.verticalCenter: parent.verticalCenter
                color: "#1d81d5"
                text: "04:23"
                font.pixelSize: 18
            }
        }
    }


    Item{
        // 1/4 bottom screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        Rectangle{

            anchors.fill: parent
            color: "grey"
        }

        Row {

            anchors.centerIn: parent
            Image {
                id: prevBtnImage
                anchors.verticalCenter: parent.verticalCenter
                source: "../res/buttons/player_prev_btn.png"
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        prevBtnImage.source = "../res/buttons/player_prev_pressed_btn.png"
                    }
                    onReleased: {
                        prevBtnImage.source = "../res/buttons/player_prev_btn.png"
                    }
                }
            }

            PlayPauseButton {}

            Image {
                id: nextBtnImage
                anchors.verticalCenter: parent.verticalCenter
                source: "../res/buttons/player_next_btn.png"
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        nextBtnImage.source = "../res/buttons/player_next_pressed_btn.png"
                    }
                    onReleased: {
                        nextBtnImage.source = "../res/buttons/player_next_btn.png"
                    }
                }
            }
        }
    }
}
