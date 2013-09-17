import QtQuick 2.0
import "../controls"

Item {
    anchors.fill: parent

    ListView {
        id: sdlPlayerOptionsListView
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width
        height:parent.height

        model: ListModel{
            ListElement{
                name: "Policies Test"
            }
            ListElement{
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

    BackBtn{anchors.top: parent.bottom}
}
