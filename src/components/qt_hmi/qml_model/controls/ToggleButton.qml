import QtQuick 2.0

Rectangle {
    id: toggleButton
    width: 160
    height: 40
    radius: 2
    border.width: 2
    border.color: "#000000"
    gradient: unPressed

    property alias label : label.text

    signal selected()
    signal pushed()

    state: "unPressed"
    onStateChanged: {
        if (state == "pressed")  {
            selected()
        } else {
            pushed()
        }
    }

    Text {
        id: label
        color: "white"
        text: "TextButton"
        font.pixelSize: 18
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.fill: parent
    }
    MouseArea {
        id: mouseArea
        cursorShape: Qt.PointingHandCursor

        anchors.fill: parent
        onPressed: {
            if (parent.state == "unPressed") {
                parent.state = "pressed"
            } else {
                parent.state = "unPressed"
            }
        }
    }

    states: [
        State  {
            name: "pressed"
            PropertyChanges  {
                target: toggleButton
                gradient: pressed
            }
        },
        State  {
            name: "unPressed"
            PropertyChanges  {
                target: toggleButton
                gradient: unPressed
            }
        }
    ]

    Gradient {
        id: unPressed

        GradientStop { position: 0.0; color: "#2c2c2c" }
        GradientStop { position: 1.0; color: "#0c0c0c" }
    }

    Gradient {
        id: pressed

        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 1.0; color: "black" }
    }

    transitions: [
        Transition  {
            from: "unPressed"
            to: "pressed"
            reversible: true
        }
    ]
}
