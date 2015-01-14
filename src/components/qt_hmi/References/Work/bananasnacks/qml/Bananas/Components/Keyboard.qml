import QtQuick 2.0

Item {
    signal letterClicked(string letter)
    signal go()
    id: keyboard

    states: [
        State {
            name: 'numpad'
            PropertyChanges {
                target: qwerty
                visible: false
            }
            PropertyChanges {
                target: numpad
                visible: true
            }
        }

    ]

    Column {
        id: qwerty
        anchors.fill: parent
        spacing: 10
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            Key { text: "Q" }
            Key { text: "W" }
            Key { text: "E" }
            Key { text: "R" }
            Key { text: "T" }
            Key { text: "Y" }
            Key { text: "U" }
            Key { text: "I" }
            Key { text: "O" }
            Key { text: "P" }
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            Key { text: "A" }
            Key { text: "S" }
            Key { text: "D" }
            Key { text: "F" }
            Key { text: "G" }
            Key { text: "H" }
            Key { text: "J" }
            Key { text: "K" }
            Key { text: "L" }
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            Key { text: "Z" }
            Key { text: "X" }
            Key { text: "C" }
            Key { text: "V" }
            Key { text: "B" }
            Key { text: "N" }
            Key { text: "M" }
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            Button { text: "123"; width: 60
                onPress: keyboard.state = 'numpad'
            }
            Button { text: "!@#"; width: 60 }
            Key { text: " "; width: 300 }
            Key { text: "." }
            Key { text: "&" }
        }
    }   

    Column {
        id: numpad
        visible: false
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

            Button {
                text: "ABC"
                width: 60
                onPress: keyboard.state = ''
            }
            Key { text: "*" }
            Key { text: "0" }
            Key { text: "#" }
            Button {
                text: "Go"
                width: 60
                onPress: keyboard.go()
            }
        }
    }
}
