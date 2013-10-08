import QtQuick 2.0

Item {
    property string text1: qsTr("Text 1")
    property string text2: qsTr("Text 2")
    property string totalDistance: qsTr("Total Distance")
    property string eta: qsTr("HH:mm")
    property url pathIcon: "../res/nav/dest_icon.png"
    property string imageIcon: ""
    property real distanceToManeuver: 1.0
    property real distanceToManeuverScale: 1.0
    property bool maneuverComplete: true
    property ListModel softButtons: ListModel {}
    property int appId: 0
}
