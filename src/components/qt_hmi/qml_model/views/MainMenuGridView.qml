import QtQuick 2.0
import "../models"

MenuGridView
{
    id: mainMenuGridview

    columns: 4
    rows: 2

    model: MainMenuListModel {}

    Component.onCompleted: {
        dataContainer.reset()
    }
}
