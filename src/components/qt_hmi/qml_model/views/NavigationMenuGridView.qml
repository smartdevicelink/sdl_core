import QtQuick 2.0
import "../controls"
import "../models"

GeneralView {
    Column {
        anchors.fill: parent

        Item {
            // 3/4 top screen
            height: parent.height * 3/4
            width: parent.width

            MenuGridView {
                id: navMenuGridView
                anchors.centerIn: parent
                model: NavigationMenuModel {}

                LongOvalButton {
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

                    LongOvalButton {
                        id: button
                        text: title
                        pixelSize: 20
                        dest: qml
                        anchors.centerIn: parent
                    }
                }
            }
        }

        Item {
            // 1/4 bottom screen
            width: parent.width
            height: 1/4 * parent.height

            BackButton { anchors.horizontalCenter: parent.horizontalCenter }
        }
    }
}
