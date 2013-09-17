import QtQuick 2.0
import com.ford.hmi_framework 1.0

Item {
    id: headerMenu
    anchors.fill: parent

    Text{
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

    Text{
        text: "12:50";
        color: "#1d81d5"
        font.pixelSize: 25;
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.topMargin: margin
        anchors.rightMargin: margin

    }

}
