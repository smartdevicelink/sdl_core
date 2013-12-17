import QtQuick 2.0
import "../Components"

Item {
    anchors.fill: parent
    anchors.topMargin: 70
    anchors.leftMargin: 20
    anchors.rightMargin: 20
    anchors.bottomMargin: 70
    property bool back_button: false

    BackButton {
        onPress: section.state = ''
        visible: parent.back_button
    }
}
