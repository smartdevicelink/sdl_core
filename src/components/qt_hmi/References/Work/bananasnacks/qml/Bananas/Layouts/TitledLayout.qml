import QtQuick 2.0
import "../Components"

EmptyLayout {
    property alias title: title.text
    anchors.topMargin: 120

    H1 {
        id: title
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.top
        anchors.bottomMargin: 10
    }
}
