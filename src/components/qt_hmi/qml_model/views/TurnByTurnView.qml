import QtQuick 2.0
import "../controls"

GeneralView {
    width: 647
    height: 384

    Rectangle {
        property string textColor: "#1d81d5"
        property string backgroundColor: "#000000"

        id: main
        color: backgroundColor
        anchors.fill: parent

        Rectangle {
            id: head
            height: 50
            border.width: 2
            border.color: parent.textColor
            color: parent.backgroundColor
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5

            Text {
                text: dataContainer.navigationModel.text1
                verticalAlignment: Text.AlignVCenter
                anchors.topMargin: 5
                anchors.bottomMargin: 5
                anchors.rightMargin: 5
                anchors.leftMargin: 10
                anchors.fill: parent
                font.pixelSize: 14
                color: parent.parent.textColor
            }
        }

        Rectangle {
            id: turnIcon
            width: 210
            height: 210
            border.color: parent.textColor
            color: parent.backgroundColor
            anchors.top: head.bottom
            anchors.topMargin: -2
            anchors.right: parent.right
            anchors.rightMargin: 5

            Image {
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                anchors.bottomMargin: 5
                anchors.topMargin: 5
                anchors.fill: parent
                source: dataContainer.navigationModel.pathIcon
            }
        }

        Rectangle {
            id: other
            width: 210
            height: 40
            color: parent.backgroundColor
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.topMargin: 5
            anchors.top: turnIcon.bottom
            Text {
                color: parent.parent.textColor
                text: dataContainer.navigationModel.text2
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
                anchors.rightMargin: 5
                anchors.topMargin: 5
                anchors.leftMargin: 10
                font.pixelSize: 14
                anchors.bottomMargin: 5
            }
            border.color: parent.textColor
        }

        OvalButton {
            id: turnList
            height: 65
            fontSize: 18
            label: "Turn List"
            anchors.top: row.bottom
            anchors.topMargin: 0
            anchors.right: turnIcon.left
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
        }

        ListView {
            id: softButton
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.right: turnIcon.left
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: turnList.bottom
            anchors.topMargin: 5
            delegate: OvalButton {
                label: text
                width: parent.width
                height: 65
            }
            model: dataContainer.navigationModel.softButtons
        }

        Rectangle {
            id: home
            color: "#ffffff"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.top: other.bottom
            anchors.topMargin: 5
            anchors.left: softButton.right
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 5
        }

        Row {
            id: row
            height: 50
            spacing: 10
            anchors.top: head.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.right: turnIcon.left
            anchors.rightMargin: 5

            Rectangle {
                id: totalDistance
                width: parent.width/2 - 5
                color: "#00000000"
                border.color: parent.parent.textColor
                anchors.bottom: parent.bottom
                anchors.top: parent.top

                Text {
                    text: dataContainer.navigationModel.totalDistance
                    verticalAlignment: Text.AlignVCenter
                    anchors.topMargin: 5
                    anchors.bottomMargin: 5
                    anchors.rightMargin: 5
                    anchors.leftMargin: 10
                    anchors.fill: parent
                    font.pixelSize: 14
                    color: parent.parent.parent.textColor
                }
            }

            Rectangle {
                id: eta
                width: parent.width/2 - 5
                color: "#00000000"
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                border.color: parent.parent.textColor

                Text {
                    color: parent.parent.parent.textColor
                    text: dataContainer.navigationModel.eta
                    verticalAlignment: Text.AlignVCenter
                    anchors.fill: parent
                    anchors.rightMargin: 5
                    anchors.topMargin: 5
                    anchors.leftMargin: 10
                    font.pixelSize: 14
                    anchors.bottomMargin: 5
                }
            }
        }
    }
}
