import QtQuick 2.0
import "../controls"
import "../models"

Column {
    anchors.fill: parent
    GridView
    {
        id: musicSourceGridView
        anchors.fill: parent
        cellWidth: width / 3
        cellHeight: height / 3

        model: MusicSourceModel {}

        populate: Transition {
            id: populateTransition;
            SequentialAnimation {
                PropertyAction  {
                    property: "opacity";
                    value: 0
                }
                PauseAnimation {
                    duration: populateTransition.ViewTransition.index * 100;
                }
                NumberAnimation {
                    properties: "opacity"
                    from: 0
                    to: 1
                    duration: 300
                }
            }
        }

        delegate: Item {
            width: musicSourceGridView.cellWidth
            height: musicSourceGridView.cellHeight

            MouseArea {
                cursorShape: Qt.PointingHandCursor
                anchors.fill: parent
                onClicked: {
                    if(qml !== "")contentLoader.go(qml)
                    if(contentLoader.item.radioType !== undefined) contentLoader.item.radioType = name;
                    if(contentLoader.item.playerType !== undefined) contentLoader.item.playerType = name;
                }
            }

            Image { source: icon; anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter;}
            Text {text: name; font.pixelSize: 25; color: "#1d81d5"; anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter;}
        }


    }
    BackBtn {anchors.bottom: parent.bottom}
}
