import QtQuick 2.0

Rectangle {
    id: rectangle1
    width: 647
    height: 384
    color: "#000000"

    Text {
        id: text1
        text: qsTr("Text 1")
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        font.pixelSize: 12
    }
}
