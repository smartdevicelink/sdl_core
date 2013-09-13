import QtQuick 2.0

Image {
    id: longOvalBtn
    source: "../res/buttons/long_oval_btn.png"
    property alias text: btnText.text
    property alias pixelSize: btnText.font.pixelSize
    property string  dest: ""
    property bool isPressed: false

    Text {
        anchors.centerIn: parent
        id: btnText
        color: "#1d81d5"
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            source = "../res/buttons/long_oval_pressed_btn.png"
            btnText.color = "black"
            isPressed = true
        }
        onReleased: {
            source = "../res/buttons/long_oval_btn.png"
            btnText.color =  "#1d81d5"
            isPressed = false
        }
        onClicked: {
            if(dest !== ""){
                menuContainer.go(dest)
            }
        }
    }
}
