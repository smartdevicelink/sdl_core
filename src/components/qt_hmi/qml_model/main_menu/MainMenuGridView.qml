import QtQuick 2.0
import ".."

MenuGridView
{
    id: mainMenuGridview

    columns: 4
    rows: 2

    model: MainMenuListModel {}
}
