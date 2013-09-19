import QtQuick 2.0

LongOvalBtn {
    text: "Back"
    pixelSize: 30
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    //anchors.bottom: parent.bottom

    onIsPressedChanged: {
        if(isPressed == false){
              contentLoader.back()
        }
    }
}
