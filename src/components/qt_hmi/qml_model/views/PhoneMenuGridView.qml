import QtQuick 2.0
import "../models"
import "../controls"

GridMenu {
    id: menu
    model: PhoneMenuModel {}
    delegate: GridItem {
        width: menu.width / menu.columnsOnPage
        height: menu.height / menu.rows
        ClickableImage {
            anchors.centerIn: parent
            source: icon
            onClicked: {
                if(qml !== "") {
                    contentLoader.go(qml)
                }
            }
        }
    }
}
