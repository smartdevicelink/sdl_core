import QtQuick 2.0

LongOvalButton {
    anchors.horizontalCenter: parent.horizontalCenter
    text: "Back"
    pixelSize: 30
    onClicked: { contentLoader.back() }
}
