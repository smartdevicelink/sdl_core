import QtQuick 2.0
import "../models"

GeneralView {
    MenuGridView {
        id: appsMenuGridview
        anchors.fill: parent
        columns: 3

        model: AppsMenuModel {}
    }
}
