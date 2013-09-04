import QtQuick 2.0

Item {
    property alias text: text.text
    property alias pressed: mousearea.pressed
    property alias textAnchors: text.anchors
    signal press

    id: container

    Text {
        id: text
        color: "white"
        font.pixelSize: 16
    }

    MouseArea {
        id: mousearea
        width: parent.width
        height: parent.height

        onClicked: container.press()
    }
}
