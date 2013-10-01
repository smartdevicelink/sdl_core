import QtQuick 2.0
import com.ford.hmi_framework 1.0

Item {
    id: headerMenu
    anchors.fill: parent
    anchors.leftMargin: 30
    anchors.rightMargin: 30
    anchors.topMargin: 30

    Text {
        text: "75°";
        color: "#1d81d5"
        font.pixelSize: 25;
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.topMargin: margin
        anchors.leftMargin: margin

    }

    Item {
        width: childrenRect.width
        height: parent.height
        anchors.horizontalCenter: headerMenu.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.topMargin: margin
        Text {
            text: "≡"
            color: "#1d81d5"
            font.pixelSize: 25
            anchors.horizontalCenter: menuText.horizontalCenter
            anchors.bottom: parent.verticalCenter
        }

        Text {
            id: menuText
            text: "MENU";
            anchors.top: parent.verticalCenter
            color: "#1d81d5"
            font.pixelSize: 25
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                contentLoader.go("./views/MainMenuGridView.qml")
            }
        }
    }

    Row {
        id: clock
        height: childrenRect.height
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.topMargin: margin
        anchors.rightMargin: margin
        property date date: { new Date(); }

        Timer {
            running: true
            repeat: true
            interval: 1000
            onTriggered: {
                clock.date = new Date();
            }
        }

        Text {
            text: Qt.formatTime(clock.date, "hh");
            color: "#1d81d5"
            font.pixelSize: 25;
        }
        Item {
            Text {
                text: clock.date.getSeconds() % 2 ? ":" : ""
                color: "#1d81d5"
                font.pixelSize: 25;
            }
            width: 10
            height: parent.height
        }

        Text {
            text: Qt.formatTime(clock.date, "mm");
            color: "#1d81d5"
            font.pixelSize: 25;
        }
    }

}
