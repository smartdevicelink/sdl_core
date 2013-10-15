import QtQuick 2.0
import "../models/Constants.js" as Constants

Item {
    default property alias content: content.children
    width: 700
    height: 500
    Rectangle {
        id: content
        width: 600
        height: 400
        color: Constants.secondaryColor
        radius: 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        border.width: 1
        border.color: "white"
    }

    function show() {
        visible = true;
    }

    function hide() {
        visible = false;
    }
}
