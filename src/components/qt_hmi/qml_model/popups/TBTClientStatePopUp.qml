import QtQuick 2.0
import QtQuick.Controls 1.0
import "../hmi_api/Common.js" as Common
import "../models/Constants.js" as Constants

PopUp {
    id: popup1
    width: 700
    height: 500

    Item {
        ListModel {
            id: tbtState

            Component.onCompleted: {
                for (var name in Common.TBTState) {
                    append({name: name});
                }
            }
        }
    }
    Text {
        text: "TBT Client State"
        verticalAlignment: Text.AlignVCenter
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        color: Constants.primaryColor
    }
    ComboBox {
        id: comboBox
        anchors.rightMargin: 100
        anchors.leftMargin: 100
        anchors.left: parent.left
        anchors.right: parent.right
        model: tbtState
        anchors.verticalCenter: parent.verticalCenter
        onCurrentTextChanged: {
            console.log("Send signal onTBTClientState:", currentText);
            sdlNavigation.onTBTClientState(Common.TBTState[currentText]);
        }
    }
}
