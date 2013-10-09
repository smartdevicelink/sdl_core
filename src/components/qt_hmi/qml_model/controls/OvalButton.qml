import QtQuick 2.0

Rectangle {
    id: main
    width: label.width * 1.5
    height: 65
    color: "#00000000"

    signal clicked;
    property alias label: label.text
    property alias fontSize: label.font.pixelSize

    Image {
        id: left
        width: 31
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        source: "../res/buttons/oval_btn_left.png"
    }

    Image {
        id: right
        x: 135
        width: 31
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        source: "../res/buttons/oval_btn_right.png"
    }

    Image {
        id: top
        height: 10
        anchors.right: parent.right
        anchors.rightMargin: 31
        anchors.left: parent.left
        anchors.leftMargin: 31
        anchors.top: parent.top
        anchors.topMargin: 0
        fillMode: Image.TileHorizontally
        source: "../res/buttons/oval_btn_top.png"
    }

    Image {
        id: bottom
        y: 50
        height: 11
        anchors.right: parent.right
        anchors.rightMargin: 31
        anchors.left: parent.left
        anchors.leftMargin: 31
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        fillMode: Image.TileVertically
        source: "../res/buttons/oval_btn_bottom.png"
    }

    Text {
        id: label
        color: "#1d81d5"
        text: qsTr("Oval Button")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        z: 50
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 16
    }

    Rectangle {
        id: background
        color: "#00000000"
        anchors.rightMargin: 31
        anchors.leftMargin: 31
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        anchors.fill: parent
    }

    MouseArea {
        id: mousearea
        z: 100
        anchors.rightMargin: 15
        anchors.leftMargin: 15
        anchors.bottomMargin: 11
        anchors.topMargin: 10
        anchors.fill: parent
        onPressed: {
            parent.state = "pressed";
        }
        onReleased: {
            parent.state = "base state";
        }
        onClicked: {
            parent.clicked();
        }

    }

    states: [
        State {
            name: "pressed"

            PropertyChanges {
                target: left
                source: "../res/buttons/oval_btn_pressed_left.png"
            }

            PropertyChanges {
                target: right
                source: "../res/buttons/oval_btn_pressed_right.png"
            }

            PropertyChanges {
                target: background
                color: "#1d81d5"
            }

            PropertyChanges {
                target: label
                color: "black"
            }
        }
    ]
}
