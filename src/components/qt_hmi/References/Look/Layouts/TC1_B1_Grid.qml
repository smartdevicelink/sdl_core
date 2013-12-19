import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {
    width: 800
    height: 480

    property alias m1: m1.children
    property alias list: listView
    property alias list_data: listView.list_data
    property alias list_itemWidget: listView.list_itemWidget
    property alias listWrap: listView.listWrap
    property alias fg: _fg



    Item {
        id: _fg

        width: 800; height: 480

        GridViewWidget {
            id: listView
            x: 135; y: 170
            width: 530; height: 200
            gridItemHeight: 100; gridItemWidth: 180

        }

        FWdgtContainer {
            id: m1
            width: 530; height: 55
        }
    }
}
