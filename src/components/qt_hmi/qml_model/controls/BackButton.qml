import QtQuick 2.0

LongOvalButton {
    text: "Back"
    pixelSize: 30
    onClicked: { contentLoader.back() }
}
