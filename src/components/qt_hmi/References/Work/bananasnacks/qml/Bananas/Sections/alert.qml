import QtQuick 2.0

Item {
    property alias text: alert_text.text
    id: alert_item
    opacity: 0

    transitions: Transition {
        NumberAnimation {
            properties: "opacity"
        }
    }

    function alert(message) {
        text = message;
        state = 'alerting';
        alert_timer.start();
    }

    states: [
        State {
            name: 'alerting'
            PropertyChanges {
                target: alert_item
                opacity: 1
            }
        }
    ]

    Timer {
        id: alert_timer
        running: false
        interval: 4000
        onTriggered: alert_item.state = '';
    }


    Rectangle {
        id: background
        anchors.fill: parent
        color: "#000"
        opacity: 0.8
    }

    Rectangle {
        color: "#111"
        width: 500
        height: alert_text.height + 40
        radius: 8
        border.color: "#666"
        border.width: 2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Text {
            id: alert_text
            text: "This is a really really long long really really long longreally really long longreally really long longreally really long long alert"
            color: "#fff"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 24
            width: parent.width
            wrapMode: Text.WordWrap
        }
    }

}
