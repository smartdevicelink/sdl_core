import QtQuick 2.0

Button {
    property alias icon: icon.source

    width: icon.width + 30

    Image {
        id: icon
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
}
