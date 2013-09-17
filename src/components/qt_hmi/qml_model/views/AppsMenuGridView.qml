import QtQuick 2.0
import "../models"

MenuGridView
{
    id: appsMenuGridview
    columns: 3

    model: AppsMenuModel {}
}
