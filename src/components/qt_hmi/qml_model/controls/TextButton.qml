import QtQuick 2.0

Rectangle {
    width: 150
    height: 50
    color: "#323232"
    border.color: "#b4b4b4"

    property alias label: text1.text

    signal clicked;

    Connections {
        target: mousearea1
        onClicked: clicked()
    }

    Text {
        id: text1
        color: "#ffffff"
        text: "TextButton"
        styleColor: "#000000"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.fill: parent
        font.pixelSize: 14
    }

    MouseArea {
        id: mousearea1
        anchors.fill: parent
    }
}
