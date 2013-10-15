import QtQuick 2.0
import "../models/Constants.js" as Constants
import "../hmi_api/Common.js" as Common
import "../views"

PopUp {
    Text {
        id: title
        height: 50
        text: "Speak the command"
        verticalAlignment: Text.AlignVCenter
        anchors.right: voice.left
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 15
        font.pixelSize: 14
        color: Constants.primaryColor
    }

    Image {
        id: voice
        x: 591
        width: 50
        height: 50
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 15
        source: "../res/controlButtons/vrImage.png"
    }

    ListView {
        id: listCommands
        anchors.bottomMargin: 15
        anchors.rightMargin: 15
        anchors.leftMargin: 15
        anchors.top: title.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0
        model: ListModel {
            ListElement {
                name: "Grey"
            }
        }
        delegate: Text {
            text: name
        }
    }

    function activate() {
        dataContainer.activeVR = true;
        dataContainer.systemSavedContext = dataContainer.systemContext
        dataContainer.systemContext = Common.SystemContext.SYSCTXT_ALERT
        sdlVR.started();
        show();
    }

    function deactivate() {
        dataContainer.activeVR = false;
        dataContainer.systemContext = dataContainer.systemSavedContext
        sdlVR.stopped();
        hide();
    }
}
