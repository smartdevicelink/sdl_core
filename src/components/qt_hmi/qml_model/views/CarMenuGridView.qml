import QtQuick 2.0
import "../models"

GeneralView {
    MenuGridView
    {
        id: carMenuGridView
        anchors.fill: parent

        columns: 4
        rows: 2

        model: CarMenuModel {}
    }
}
