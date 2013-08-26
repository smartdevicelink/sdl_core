import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {
    width: 800
    height: 115

    property alias m1: m1.children
    property alias list: listView
    property alias list_data: listView.list_data
    property alias list_itemWidget: listView.list_itemWidget
    property alias listWrap: listView.listWrap
    property alias fg: _fg
    property alias gridCurItem: listView.gridCurItem
    property alias moving: listView.moving
    property alias direction: listView.direction



    Item {
        id: _fg

        width: 800; height: 115

        GridViewPresetWidget {
            id: listView
            x: 0; y: 0
            width: 800; height: 115
            gridItemHeight: 115; gridItemWidth: 200

        }

        FWdgtContainer {
            id: m1
            width: 676; height: 115
        }
    }
}
