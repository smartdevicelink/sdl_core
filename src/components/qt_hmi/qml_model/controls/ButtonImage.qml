import QtQuick 2.0

Image {
    property string name
    property alias pressed : pressedImg.source
    property alias pressedOpacity: pressedImg.opacity
    Image {
        id: pressedImg
        opacity: 0
        Behavior on opacity {
            NumberAnimation { duration: 80 }
        }
    }
}
