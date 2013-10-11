import QtQuick 2.0
import com.ford.sdl.hmi.hw_buttons 1.0
import "../models/Constants.js" as Constant

Item {
    id: headerMenu
    anchors.fill: parent
    anchors.leftMargin: Constant.margin
    anchors.rightMargin: Constant.margin
    anchors.topMargin: Constant.margin

    Text {        
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.topMargin: Constant.margin
        anchors.leftMargin: Constant.margin
        color: Constant.primaryColor
        font.pixelSize: Constant.fontSize
        text: "75°"
    }

    Item {
        width: childrenRect.width
        height: parent.height
        anchors.horizontalCenter: headerMenu.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.topMargin: Constant.margin
        Text {
            anchors.horizontalCenter: menuText.horizontalCenter
            anchors.bottom: parent.verticalCenter
            text: "≡"
            color: Constant.primaryColor
            font.pixelSize: Constant.fontSize
        }

        Text {
            id: menuText
            anchors.top: parent.verticalCenter
            text: "MENU";            
            color: Constant.primaryColor
            font.pixelSize: Constant.fontSize
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                contentLoader.go("./views/MainMenuView.qml")
            }
        }
    }

    Row {
        id: clock
        height: childrenRect.height
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.topMargin: Constant.margin
        anchors.rightMargin: Constant.margin
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
            color: Constant.primaryColor
            font.pixelSize: Constant.fontSize;
        }

        Item {
            width: 10
            height: parent.height
            Text {
                text: clock.date.getSeconds() % 2 ? ":" : ""
                color: Constant.primaryColor
                font.pixelSize: Constant.fontSize;
            }
        }

        Text {
            text: Qt.formatTime(clock.date, "mm");
            color: Constant.primaryColor
            font.pixelSize: Constant.fontSize;
        }
    }

}
