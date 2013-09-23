import QtQuick 2.0
import "../controls"
import "../models"

Item {
    anchors.fill: parent

    Item {
        // 3/4 top screen
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left

        Text {
            width: parent.width
            id: searchDeviceText
            color: "#1d81d5"
            font.pixelSize: 40
        }

        ListView {
            id: changeDeviceListView
            model: dataContainer.deviceList

            y: searchDeviceText.height
            width:parent.width
            height:parent.height - searchDeviceText.height

            delegate: Text  {
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        sdlBasicCommunications.onFindApplications({ name: name, id: devid })
                        contentLoader.go("./views/ApplicationListView.qml")
                    }
                }
                text: name
                color: "#1d81d5"
                font.pixelSize: 50
            }
        }
    }
    Item {
        // 1/4 bottom screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        BackButton { anchors.horizontalCenter: parent.horizontalCenter }
    }
}
