import QtQuick 2.0

Rectangle {
    id: mainScreen
    property string backURI: ""
    property string currentURI: ""
    width: 1360
    height: 768
    color: "black"

    MouseArea {
        anchors.fill: parent
        onClicked: {
            //Qt.quit();
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
            Text{
                text: "__"
                color: "#1d81d5"
                font.pixelSize: 25
                anchors.horizontalCenter: parent.horizontalCenter
                y: parent.y - 55
            }
            Text{
                text: "__"
                color: "#1d81d5"
                font.pixelSize: 25
                anchors.horizontalCenter: parent.horizontalCenter
                y: parent.y - 60
            }
            Text{
                text: "__"
                color: "#1d81d5"
                font.pixelSize: 25
                anchors.horizontalCenter: parent.horizontalCenter
                y: parent.y - 65
            }

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
        width: 884
        height: 298

        onSourceChanged: {
            mainScreen.backURI = mainScreen.currentURI
            mainScreen.currentURI = source
        }
    }

    PowerSwitcher {
        x: parent.x + 50
        anchors.verticalCenter: parent.verticalCenter
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

}

