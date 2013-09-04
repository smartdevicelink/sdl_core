import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {
    id: layout
    width: 800
    height: 480

    property alias m1: m1.children
    property alias list: listView
    property alias list_data: listView.list_data
    property alias list_itemWidget: listView.list_itemWidget
    property alias listWrap: listView.listWrap
    property alias fg: _fg
    property alias theX: listView.x
    property alias theY: listView.y
    property alias theWidth: listView.width
    property alias theHeight: listView.height
    property alias itemWidth: listView.gridItemWidth
    property alias itemHeight: listView.gridItemHeight
    property alias close: closeTouch
    property bool useClose: true




    Item {
        id: _fg

        width: 800; height: 480

        GridViewWidget {
            id: listView
            x: 130; y: 170
            width: 540; height: 200
            gridItemHeight: 100; gridItemWidth: 180

        }

        FWdgtContainer {
            id: m1
            width: 530; height: 55
        }


    }

    BaseText{
        id: closeTxt
        anchors.horizontalCenter: layout.horizontalCenter
        y:430
        horAlignment: Text.AlignHCenter
        vertAlignment: Text.AlignTop
        text: "CLOSE"
        color: "#1d81d5"
        pxSize: 22
        theFont: "../Fonts/HelveticaNeueLTStd-Md.ttf"
        visible: useClose

    }

    FWdgtTouchArea{
        id: closeTouch
        height: 75
        width: 750
        y:430
        anchors.horizontalCenter: layout.horizontalCenter
        visible: useClose
        enabled: useClose
    }
}
