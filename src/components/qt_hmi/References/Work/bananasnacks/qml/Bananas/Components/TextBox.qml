import QtQuick 2.0

Rectangle {
    property alias text: text.text
    property alias icon: icon.source

    border.width: 2
    color: 'black'
    height: 42
    radius: 3

    Image {
        id: icon
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
    }

    TextInput {
        id: text
        color: "white"
        font.pixelSize: 24
        anchors.left: icon.right
        anchors.right: parent.right
        anchors.leftMargin: 20
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }
}
