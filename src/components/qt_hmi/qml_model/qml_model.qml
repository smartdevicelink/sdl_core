import QtQuick 2.0
import com.ford.hmi_framework 1.0

Rectangle {
    id: mainScreen
    width: 1520
    height: 768
    color: "black"

    MouseArea {
        anchors.fill: parent
        onClicked: {
            //Qt.quit();
        }
    }

    PowerSwitcher {
        x: parent.x + 50
        anchors.verticalCenter: parent.verticalCenter
        onSwitched: {
            if (sw === "on") {
                mainScreen.state = "Main"
                menuContainer.go("AMFMPlayerView.qml")
                menuContainer.item.radioType = "AM"
            } else {
                mainScreen.state = ""
                menuContainer.source = ""
            }
        }
    }

    Item {
        id: headerMenu
        x: menuContainer.x + menuContainer.width / 2 - 400
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

        Text {
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    menuContainer.go("MainMenuGridView.qml")
                }
            }
            Text {
                text: "≡"
                color: "#1d81d5"
                font.pixelSize: 25
                anchors.horizontalCenter: parent.horizontalCenter
                y: parent.y - 55
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

    Loader {
        id: menuContainer
        x: 160
        anchors.verticalCenter: parent.verticalCenter
        width: 780
        height: 298


        ListModel {
            id: urlStack
        }

        function go(path) {
            urlStack.append({ url: source.toString(), index: 10 })
            source = path
        }

        function back() {
            var item = urlStack.get(urlStack.count - 1)
            source = item.url
            urlStack.remove(item)
        }
    }

    HardwareButtons {
        id: hardwareButtons

        x: menuContainer.x + menuContainer.width + 20
        anchors.verticalCenter: parent.verticalCenter
    }
}

