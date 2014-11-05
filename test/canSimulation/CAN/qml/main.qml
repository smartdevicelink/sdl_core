import QtQuick 2.2
import QtQuick.Controls 1.1
import "qrc:///qml/components" 1.0

ApplicationWindow {

    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    TabView {

        id: viewsTab
        width: parent.width
        height: 400

        Tab {
            title: "Media"
            Media {
                id: mediaComponent

            }
        }
        Tab {
            title: "Climate"
            Rectangle { color: "blue" }
        }
        Tab {
            title: "Navigation"
            Rectangle { color: "green" }
        }
        Tab {
            title: "Phone"
            Rectangle { color: "yellow" }
        }
        Tab {
            title: "MCS"
            Rectangle { color: "white" }
        }
        Tab {
            title: "Settings"
            Rectangle { color: "black" }
        }
    }

    TextArea {

        id: tcpLogsView
        width: parent.width
        anchors.top: viewsTab.bottom
        height: parent.height - viewsTab.height
        visible: true
        text:
            "Lorem ipsum dolor sit amet, consectetur adipisicing elit, " +
            "sed do eiusmod tempor incididunt ut labore et dolore magna " +
            "aliqua. Ut enim ad minim veniam, quis nostrud exercitation " +
            "ullamco laboris nisi ut aliquip ex ea commodo cosnsequat. ";
    }
}
