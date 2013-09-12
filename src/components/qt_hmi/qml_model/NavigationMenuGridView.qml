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

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    width: 600
    height: 150

    property int columns: 3
    property int rows: 2

    cellWidth: width / columns
    cellHeight: height / rows

    anchors.centerIn: parent

    delegate: Item {
        width: 200
        height: 80

        Image { source: icon; anchors.centerIn: parent }
        Text {color: "steelblue"; text: title; anchors.centerIn: parent }
        MouseArea {
            cursorShape: Qt.PointingHandCursor
            anchors.fill: parent
            onClicked: {
                menuContainer.source = qml
            }
        }
    }

}
}
