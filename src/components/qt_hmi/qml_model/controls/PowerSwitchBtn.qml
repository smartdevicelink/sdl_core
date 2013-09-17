import QtQuick 2.0

Rectangle {
    id: powerSwitchBtn
    state: "OFF"
    width: 80
    height: 80
    radius: 5
    property bool pressed;

    gradient: Gradient {
        GradientStop { position: pressed ? 1.0 : 0.0; color: "#2c2c2c" }
        GradientStop { position: pressed ? 0.0 : 1.0; color: "black" }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            parent.pressed = true
        }
        onReleased: {
            parent.pressed = false
            parent.state = parent.state === "ON" ? "OFF" : "ON"
            btnText.text = parent.state
            if(parent.state === "ON")
                mainScreen.visible = true
            else
                mainScreen.visible = false
        }
    }

    Text {
        id: btnText
        color: "red"
        text: parent.state
        anchors.centerIn: parent
        font.pixelSize: 40
    }

}
