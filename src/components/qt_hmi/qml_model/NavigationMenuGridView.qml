import QtQuick 2.0
import "./controls"

Item {
    anchors.fill: parent

    MenuGridView {
        anchors.centerIn: parent
        id: navMenuGridView
        model: NavigationMenu {}

        property int buttonWidth: 163
        property int buttonHeight: 65

        width: 3*buttonWidth
        height: 2*buttonHeight
        columns: 3
        rows: 2

        delegate: Item {
            id: cell
            width: navMenuGridView.cellWidth
            height: navMenuGridView.cellHeight

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
        font.pixelSize: 20
        MouseArea {
          anchors.fill: parent
          onClicked: {
              menuContainer.go("NavigationNoRouteGridView.qml")
            }
        }

    }
}
