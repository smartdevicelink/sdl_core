import QtQuick 2.0

Rectangle {
    id: toggleButton
    width: 160
    height: 40
    radius: 2
    border.width: 2
    border.color: "#000000"
    gradient: grUnpressed

    property alias label : label.text

    signal pressed()
    signal unpressed()
    signal clicked()

    property bool toggleMode: false

    state: "unpressed"
    onStateChanged: {
        if (state == "pressed") {
            pressed();
        } else {
            unpressed();
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

        anchors.fill: parent
        onPressed: {
            if (toggleMode) {
                if (parent.state == "unpressed") {
                    parent.state = "pressed"
                } else {
                    parent.state = "unpressed"
                }
            } else {
                parent.state = "pressed"
            }
        }
        onReleased: {
            if (!toggleMode) {
                parent.state = "unpressed"
                if (mouse.isClick) {
                    parent.clicked()
                }
            }
        }
    }

    states: [
        State  {
            name: "pressed"
            PropertyChanges  {
                target: toggleButton
                gradient: grPressed
            }
        },
        State  {
            name: "unpressed"
            PropertyChanges  {
                target: toggleButton
                gradient: grUnpressed
            }
        }
    ]

    Gradient {
        id: grUnpressed

        GradientStop { position: 0.0; color: "#2c2c2c" }
        GradientStop { position: 1.0; color: "#0c0c0c" }
    }

    Gradient {
        id: grPressed

        GradientStop { position: 0.0; color: "black" }
        GradientStop { position: 1.0; color: "black" }
    }

    transitions: [
        Transition  {
            from: "unpressed"
            to: "pressed"
            reversible: true
        }
    ]
}
