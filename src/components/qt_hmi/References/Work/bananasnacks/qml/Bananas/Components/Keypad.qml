import QtQuick 2.0

Item {
    signal letterClicked(string letter)
    id: keyboard

    Column {
        anchors.fill: parent
        spacing: 10
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            Key { text: "1" }
            Key { text: "2" }
            Key { text: "3" }
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            Key { text: "4" }
            Key { text: "5" }
            Key { text: "6" }
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            Key { text: "7" }
            Key { text: "8" }
            Key { text: "9" }
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            Key { text: "*" }
            Key { text: "0" }
            Key { text: "#" }
        }
    }
}
