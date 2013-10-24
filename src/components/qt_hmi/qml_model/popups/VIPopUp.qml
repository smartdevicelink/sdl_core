//!!

import QtQuick 2.0
import "../models/Constants.js" as Constants
import "../hmi_api/Common.js" as Common
import "../controls"
import QtQuick.Controls 1.0

PopUp {
    id: popUP
    anchors.margins: Constants.margin
    //TODO{ALeshin}: Redraw this window as ListView or Column, when we'll get requirements
    Column {
        anchors.fill: parent
        spacing: 0
        Text {
            id: viText
            width: parent.width
            height: 1/5 * parent.height
            text: "Vehicle Information"
            font.pixelSize: Constants.fontSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: Constants.primaryColor
        }

        Row {
            height: 1/5 * parent.height
            width: parent.width

            Text {
                width: 1/4 * parent.width
                height: 1/5 * parent.height
                anchors.verticalCenter: parent.verticalCenter
                text: "PRNDL"
                font.pixelSize: Constants.fontSize
                verticalAlignment: Text.AlignVCenter
                color: Constants.primaryColor
            }

            ComboBox {
                anchors.verticalCenter: parent.verticalCenter
                id: comboBox
                model: [ "PARK", "REVERSE","NEUTRAL","DRIVE","SPORT","LOWGEAR","FIRST","SECOND",
                    "THIRD","FOURTH","FIFTH","SIXTH"]
                onCurrentIndexChanged: {dataContainer.vehicleInfoModel.prndl = currentIndex}
                Component.onCompleted: {comboBox.currentIndex = dataContainer.vehicleInfoModel.prndl}
            }
        }

        Text {
            width: parent.width
            height: 1/5 * parent.height
            text: "Speed: " + dataContainer.vehicleInfoModel.speed
            font.pixelSize: Constants.fontSize
            verticalAlignment: Text.AlignVCenter
            color: Constants.primaryColor
        }

        Text {
            width: parent.width
            anchors.leftMargin: 500
            height: 1/5 * parent.height
            text: "Revolutions per minute: " + dataContainer.vehicleInfoModel.rpm
            font.pixelSize: Constants.fontSize
            verticalAlignment: Text.AlignVCenter
            color: Constants.primaryColor
        }
    }
}
