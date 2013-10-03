import QtQuick 2.0
import "../controls"
import "../models"

GeneralView {
    GridView
    {
        id: musicSourceGridView
        height: 3 / 4 * parent.height
        width: parent.width
        cellWidth: width / 3
        cellHeight: height / 4

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

        delegate:
            Item {
                id: item
                width: musicSourceGridView.cellWidth
                height: musicSourceGridView.cellHeight

                LongOvalButton {
                    anchors.centerIn: item
                    text: name
                    pixelSize: 25
                    dest: qml
                }
        }

    }
    Item {
        // 1/4 bottom screen
        width: parent.width
        height: 1/4 * parent.height
        anchors.bottom: parent.bottom

        BackButton { anchors.horizontalCenter: parent.horizontalCenter }
    }
}
