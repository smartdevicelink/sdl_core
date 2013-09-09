import QtQuick 2.0

Rectangle {
    id: mainScreen
    width: 1360
    height: 768
    color: "black"

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    Item {
        id: headerMenu
        x: parent.width / 2 - 400
        y: 120
        width: 800
        height: 80
        Text{
            text: "75°";
            color: "#1d81d5"
            font.pixelSize: 25;
            width: parent.width / parent.columns
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }
        Text{
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    menuContainer.source = "MainMenuGridView.qml"
                }
            }

            text: "MENU";
            color: "#1d81d5"
            font.pixelSize: 25
            width: parent.width / parent.columns
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        Text{
            text: "12:50";
            color: "#1d81d5"
            font.pixelSize: 25;
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }

        visible: mainScreen.state !== ""
    }

    property Item mmm;

    Loader {
        id: menuContainer
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
    }

    PowerSwitcher {
        onSwitched: {
            if (sw === "on") {
                mainScreen.state = "Main"
                menuContainer.source = "MainMenuGridView.qml"
            } else {
                mainScreen.state = ""
                menuContainer.source = ""
            }
        }
    }

    Text {
        id: caption
        color: "lightblue"
        text: "Hello"
    }

    states: [
        State {
            name: "Main"
        },
        State {
            name: "Car"
        }
    ]
}

