import QtQuick 2.0
import "../models"
import "../controls"
import "../models/Constants.js" as Constants

GeneralView {
    applicationContext: true

    Item {
        // top 3/4 screen
        id: upperContent
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left

        Text {
            id: deviceName
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 1/4 * parent.height
            verticalAlignment: Text.AlignHCenter
            text: "Device: " + dataContainer.currentApplication.deviceName
            font.pixelSize: Constants.titleFontSize
            color: Constants.primaryColor
        }

        Row {
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: parent.width
            height: parent.height * 3/4
            spacing: Constants.margin

            Image {
                id: image
                height: parent.height
                width: height
                source: "../res/album_art.png"
            }

            Column {
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                width: parent.width - image.width - parent.spacing
                spacing: (height - 4 * text1.height) / 3

                Text {
                    id: text1
                    verticalAlignment: Text.AlignHCenter
                    text: dataContainer.currentApplication.hmiUIText.mainField1
                    font.pixelSize: Constants.fontSize
                    color: Constants.primaryColor
                }

                Text {
                    verticalAlignment: Text.AlignHCenter
                    text: dataContainer.currentApplication.hmiUIText.mainField2
                    font.pixelSize: Constants.fontSize
                    color: Constants.primaryColor
                }

                Text {
                    verticalAlignment: Text.AlignHCenter
                    text: dataContainer.currentApplication.hmiUIText.mainField3
                    font.pixelSize: Constants.fontSize
                    color: Constants.primaryColor
                }

                Text {
                    verticalAlignment: Text.AlignHCenter
                    text: dataContainer.currentApplication.hmiUIText.mainField4
                    font.pixelSize: Constants.fontSize
                    color: Constants.primaryColor
                }
            }
        }
    }

    Item {
        // bottom 1/4 screen
        id: lowerContent
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: parent.height * 1/4

        BackButton { anchors.centerIn: parent }
    }
}
