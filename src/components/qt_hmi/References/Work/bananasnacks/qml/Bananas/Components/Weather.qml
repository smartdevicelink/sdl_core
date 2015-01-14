import QtQuick 2.0

Item {
    property alias text: text.text
    property alias temp: temp.text
    property alias image: image.source

    width: 150
    height: 75

    Text {
        id: text
        text: "Today"
        anchors.top: parent.top
        color: "#ffffff"
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Text {
        id: temp
        text: "72"
        font.pixelSize: 36
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        color: "#ffffff"
    }

    Image {
        id: image
        source: "../Assets/Cloud.png"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }
}
