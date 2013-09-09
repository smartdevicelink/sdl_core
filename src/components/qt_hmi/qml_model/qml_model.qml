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
            text: "MENU";
            color: "#1d81d5"
            font.pixelSize: 25;
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

    Item {
        id: menuContainer
    }

    PowerSwitcher {
        onSwitched: {
            if (sw === "on")
                mainScreen.state = "Main"
            else
                mainScreen.state = ""
        }
    }

    Text {
        id: caption
        color: "lightblue"
        text: "Hello"
    }

    MainMenuGridView {
        id: mainMenuGridView
    }
    CarMenuGridView {
        id: carMenuGridView
    }

    AppsMenuGridView {
        id: appsMenuGridView
    }

    states: [
        State {
            name: "Main"
        },
        State {
            name: "Car"
        },
        State {
            name: "Apps"
        }
    ]
}

