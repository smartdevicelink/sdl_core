import QtQuick 2.0

Item{

GridView {
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

    width: 600
    height: 150

    property int columns: 3
    property int rows: 2

    cellWidth: width / columns
    cellHeight: height / rows

    anchors.centerIn: parent

    delegate: Item {
        width: navigationMenuGridView.cellWidth
        height: navigationMenuGridView.cellHeight

        Image { source: icon; anchors.centerIn: parent }
        Text {color: "#1d81d5"; text: title; anchors.centerIn: parent; font.pixelSize: 20 }
        MouseArea {
            cursorShape: Qt.PointingHandCursor
            anchors.fill: parent
            onClicked: {
                menuContainer.source = qml
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
}
