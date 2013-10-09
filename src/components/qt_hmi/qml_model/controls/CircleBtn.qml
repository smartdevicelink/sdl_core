import QtQuick 2.0

Image {
    id: circleBtn
    source: imgOff
    property alias text: btnText.text
    property alias pixelSize: btnText.font.pixelSize
    property string  dest: ""
    property bool isPressed: false
    property string imgOff: "../res/buttons/round_btn.png"
    property string imgOn: "../res/buttons/round_pressed_btn.png"
    property string textColorOnPressed: "black"
    property string textColorDefault: "#1d81d5"

    signal clicked()
    function wasClicked()
    {
        clicked()
    }

    Text {
        anchors.centerIn: parent
        id: btnText
        color: textColorDefault
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            source = imgOn
            btnText.color = textColorOnPressed
            isPressed = true
        }
        onReleased: {
            source = imgOff
            btnText.color =  textColorDefault
            isPressed = false
        }
        onClicked: {
            if(dest !== ""){
                contentLoader.go(dest)
            }
            circleBtn.wasClicked()
        }
    }
}
