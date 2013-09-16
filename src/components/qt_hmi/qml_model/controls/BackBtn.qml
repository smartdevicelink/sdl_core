import QtQuick 2.0

LongOvalBtn {
    text: "Back"
    pixelSize: 30
    anchors.horizontalCenter: parent.horizontalCenter
    //anchors.bottom: parent.bottom

    onIsPressedChanged: {
        if(isPressed == false){
              menuContainer.back()
        }
    }
}
