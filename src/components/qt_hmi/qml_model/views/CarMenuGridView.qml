import QtQuick 2.0
import "../models"

MenuGridView
{
    id: carMenuGridView

    columns: 4
    rows: 2

    model: CarMenuModel {}
}
