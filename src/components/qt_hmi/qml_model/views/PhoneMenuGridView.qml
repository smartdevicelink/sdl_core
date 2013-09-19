import QtQuick 2.0
import "../models"

MenuGridView
{
    id: phoneMenuGridView

    columns: 4
    rows: 2

    model: PhoneMenuModel {}
}
