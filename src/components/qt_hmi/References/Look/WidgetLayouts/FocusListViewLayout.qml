// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {
    id: theLayout

    property alias list : theListView
    property alias list_itemWidget: theListView.itemWidget
    property alias list_data: theListView.data
    property bool listWrap: false

    FWdgtListView {
        id: theListView
        anchors.fill: theLayout
        clip: true
        //interactive: false
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 100
        highlightMoveVelocity: -1
        keyNavigationWraps: listWrap
        highlight:
            Rectangle {
                id: focus
                height: 51
                width: theLayout.width
                radius: 10
                color: "transparent"
                border { color: "white"; width: 2 }
        }
    }
}
