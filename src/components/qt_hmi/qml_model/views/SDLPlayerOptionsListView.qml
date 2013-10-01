import QtQuick 2.0
import "../controls"

GeneralView {
    ListView {
        id: sdlPlayerOptionsListView
        anchors.fill: parent

        model: ListModel {
            ListElement {
                name: "Policies Test"
            }
            ListElement {
                name: "XML Test"
            }
        }


        delegate: Text  {
            text: name
            color: "#1d81d5"
            font.pixelSize: 40
            MouseArea {
              anchors.fill: parent
              onClicked: {
                }
            }
        }
    }

    BackButton { anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter }
}
