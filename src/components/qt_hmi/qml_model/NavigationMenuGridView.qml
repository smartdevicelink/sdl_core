import QtQuick 2.0
import "./controls"

Item{
    anchors.fill: parent

    GridView {
        anchors.fill: parent
        id: navigationMenuGridView
        model: NavigationMenu {}

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

        x: (parent.width - width) / 2
        y: (parent.height - height) / 3


        property int columns: 3
        property int rows: 3

        cellWidth: width / columns
        cellHeight: height / rows

        anchors.centerIn: parent

        delegate: Item {
            width: navigationMenuGridView.cellWidth
            height: navigationMenuGridView.cellHeight

            LongOvalBtn {
                text: title
                pixelSize: 20
                dest: qml
                anchors.centerIn: parent
            }
        }

   }
    Text{
        id: closeText
        anchors.horizontalCenter: parent.horizontalCenter
        y:250
        text: "CLOSE"
        color: "#1d81d5"
        font.pixelSize: 25
        MouseArea {
          anchors.fill: parent
          onClicked: {
              menuContainer.go("NavigationNoRouteGridView.qml")
            }
        }
    }

}
