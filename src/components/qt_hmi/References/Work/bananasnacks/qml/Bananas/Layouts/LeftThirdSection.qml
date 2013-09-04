import QtQuick 2.0
import "../Components"

FocusScope {
    anchors.fill: parent
    property alias header: header.text

    Text {
      id: header
      font.pixelSize: 24
      color: "#ffffff"
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.top: parent.top
      anchors.topMargin: 70
    }
}
