import QtQuick 2.0
import "../controls"

Rectangle {
    property string textColor: "#1d81d5"
    property string backgroundColor: "#000000"
    property string navigationText1: qsTr("Text 1")
    property string navigationText2: qsTr("Text 2")
    property string totalDistance: qsTr("Total Distance")
    property string eta: qsTr("HH:mm")
    property url source: "../res/nav/dest_icon.png"

    id: main
    width: 647
    height: 384
    color: backgroundColor

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
            text: parent.parent.navigationText1
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
        id: totalDistance
        width: 200
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 5
        border.color: parent.textColor
        anchors.top: head.bottom
        anchors.topMargin: 10
        color: parent.backgroundColor

        Text {
            text: parent.parent.totalDistance
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
        id: eta
        x: 4
        height: 50
        anchors.left: totalDistance.right
        anchors.topMargin: 10
        anchors.leftMargin: 5
        anchors.top: head.bottom
        color: parent.backgroundColor
        anchors.right: parent.right
        anchors.rightMargin: 237
        border.color: parent.textColor

        Text {
            color: parent.parent.textColor
            text: parent.parent.eta
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
            anchors.rightMargin: 5
            anchors.topMargin: 5
            anchors.leftMargin: 10
            font.pixelSize: 14
            anchors.bottomMargin: 5
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
            source: parent.parent.source
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
            text: parent.parent.navigationText2
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
        anchors.top: totalDistance.bottom
        anchors.topMargin: 5
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
            label: name
            width: parent.width
        }
        model: ListModel {
            ListElement {
                name: "Reply"
            }

            ListElement {
                name: "Close"
            }
        }
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
}
