import QtQuick 2.0
import "../Components"

FocusScope {
    anchors.fill: parent
    property alias header: header.text
    property alias artwork: artwork.source
    property alias title: title.text
    property alias artist: artist.text

    Text {
        id: title
        text: ""
        color: "#ffffff"
        anchors.top: source_select.bottom
        anchors.topMargin: 20
        font.pixelSize: 28
        anchors.left: parent.left
        anchors.leftMargin: 20
    }
    Text {
        id: artist
        text: ""
        color: "#ffffff"
        anchors.top: title.bottom
        anchors.topMargin: 10
        font.pixelSize: 18
        anchors.left: title.left
    }

    Text {
      id: header
      font.pixelSize: 24
      color: "#ffffff"
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.top: parent.top
      anchors.topMargin: parent.height / 20 + 60
    }

    Button {
        id: source_select
        text: "Select source"
        x: 20
        y: 75
        onPress: section.state = 'menu'
    }

    Image {
        id: artwork
        anchors.right: parent.right
        anchors.rightMargin: 40
        anchors.top: parent.top
        width: 230
        height: 230
        anchors.topMargin: 140
        source: ""
    }

}
