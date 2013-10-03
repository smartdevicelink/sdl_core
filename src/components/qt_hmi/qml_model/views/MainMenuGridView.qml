import QtQuick 2.0
import "../models"

GeneralView {
    MenuGridView
    {
        id: mainMenuGridview
        anchors.fill: parent

        columns: 4
        rows: 2

        model: MainMenuListModel {}

        Component.onCompleted: {
            dataContainer.reset()
        }
    }
}
