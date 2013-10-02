import QtQuick 2.0
import "../controls"
import "../models"

//TODO: (ALeshin) Buttons doesnt changes when pressed

Column {
    anchors.fill: parent
    GridView
    {
        height: 3 / 4 * parent.height
        width: parent.width
        id: musicSourceGridView
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
                    if(qml !== "")
                        contentLoader.go(qml)
                }
            }

            Image { source: icon; anchors.centerIn: parent }
            Text { text: name; font.pixelSize: 25; color: "#1d81d5"; anchors.centerIn: parent }
        }


    }
    Item {
        // 1/4 bottom screen
        width: parent.width
        height: 1/4 * parent.height

        BackButton { anchors.horizontalCenter: parent.horizontalCenter }
    }
}
