import QtQuick 2.0
import "./controls"

Item {
    anchors.fill: parent

    MenuGridView {
        anchors.centerIn: parent
        id: navMenuGridView
        model: NavigationMenu {}

        LongOvalBtn {
            id: lob
            visible: false
        }

        width: 3*lob.width
        height: 2*lob.height

        columns: 3
        rows: 2

        delegate: Item {
            id: cell
            width: button.width
            height: button.height

            LongOvalBtn {
                id: button
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
