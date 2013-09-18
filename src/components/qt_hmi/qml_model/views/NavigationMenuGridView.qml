import QtQuick 2.0
import "../controls"
import "../models"

Item {
    id: navMenu
    anchors.fill: parent

    Item {
        // 3/4 top screen
        height: parent.height * 3/4
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        MenuGridView {
            id: navMenuGridView
            anchors.centerIn: parent
            model: NavigationMenuModel {}

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
    }

    Item{
        // 1/4 bottom screen
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width
        height: 1/4 * parent.height

        BackBtn {
            onIsPressedChanged: {
                if(isPressed == false){
                    contentLoader.go("./views/NavigationNoRouteGridView.qml")
                }
            }
        }
    }
}
