import QtQuick 2.0

Rectangle {
    signal press
    property alias text: clickable.text
    property alias textAnchors: clickable.textAnchors
    property alias clickable: clickable
    id: container
    border.color: "#333"
    border.width: 1
    height: 45
    width: 150
    radius: 3
    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: clickable.pressed ?  "#3C1414" : Qt.rgba(0.18, 0.18, 0.18, 1)
        }
        GradientStop {
            position: 1.0
            color: clickable.pressed ? "#9C3335" : "black"
        }
    }
    Clickable {
        anchors.fill: parent
        id: clickable
        onPress: container.press()

        textAnchors.horizontalCenter: horizontalCenter
        textAnchors.verticalCenter: verticalCenter
    }
}
