import QtQuick 2.0
import "../Components"

Item {
    id: container
    anchors.fill: parent

    property int horizontalLabelMargin: 20

    Image {
        id: phonetab
        source: "../Assets/PhoneTab.png"
        anchors.top: parent.top
        anchors.topMargin: (root.state === "phone") ? -height : 0
        Behavior on anchors.topMargin { PropertyAnimation {} }
        width: parent.width / 2 - 145
        height: parent.height / 20 + 25

        Clickable {
            anchors.fill: parent
            text: "Phone"
            onPress: go("phone")
            textAnchors.verticalCenter: verticalCenter
            textAnchors.leftMargin: horizontalLabelMargin
            textAnchors.left: left
        }
    }
    Row {
        anchors.right: phonetab.right
        anchors.rightMargin: phonetab.width / 4
        anchors.top: parent.top
        height: phonetab.height
        spacing: 10

        Image {
            source: "../Assets/PhoneCornerIcons.png"
            anchors.verticalCenter: parent.verticalCenter
        }
    }


    Image {
        source: "../Assets/NavigationTab.png"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: (root.state === "navigation") ? -height : 0
        Behavior on anchors.topMargin { PropertyAnimation {} }
        width: parent.width / 2 - 145
        height: parent.height / 20 + 25

        Clickable {
            anchors.fill: parent
            text: "Navigation"
            onPress: go("navigation")
            textAnchors.verticalCenter: verticalCenter
            textAnchors.rightMargin: horizontalLabelMargin
            textAnchors.right: right
        }
    }

    Image {
        id: test1
        source: "../Assets/MediaTab.png"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: (root.state === "media") ? -height : 0
        anchors.left: parent.left
        Behavior on anchors.bottomMargin { PropertyAnimation {} }
        width: parent.width / 2 - 145
        height: parent.height / 20 + 25

        Clickable {
            anchors.fill: parent
            text: "Media"
            onPress: go("media")
            textAnchors.verticalCenter: verticalCenter
            textAnchors.leftMargin: horizontalLabelMargin
            textAnchors.left: left
        }
    }

    Image {
        source: "../Assets/VehicleTab.png"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: (root.state === "vehicle") ? -height : 0
        Behavior on anchors.bottomMargin { PropertyAnimation {} }
        width: parent.width / 2 - 145
        height: parent.height / 20 + 25

        Clickable {
            anchors.fill: parent
            text: "Vehicle"
            onPress: go("vehicle")
            textAnchors.verticalCenter: verticalCenter
            textAnchors.rightMargin: horizontalLabelMargin
            textAnchors.right: right
        }
    }

    Image {
        id: chrometop
        source: "../Assets/ChromeTop.png"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height / 18 + 35
        width: parent.width / 2 + 40
        fillMode: Image.PreserveAspectFit
        Clickable {
            onPress: go((root.state === 'home_corners') ? 'home' : 'home_corners')
            width: parent.width / 5
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: (parent.width - parent.paintedWidth)/2 + 20
        }

        Text {
            id: clock
            text: "12:00"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 10
            color: "#ffffff"
            font.pixelSize: 28
        }
    }
    Image {
        source: "../Assets/ChromeBottom.png"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height / 18 + 35
        width: parent.width / 2
        fillMode: Image.PreserveAspectFit

        Text {
            id: exterior_temp
            text: "72F Outside"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            color: "#ffffff"
            anchors.bottomMargin: 15
            font.pixelSize: 18
        }
    }
}
