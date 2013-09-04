// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {
    id: theLayout

    property alias list : theGridView
    property alias list_itemWidget: theGridView.itemWidget
    property alias list_data: theGridView.data
    property bool listWrap: false
    property alias gridItemHeight: theGridView.cellHeight
    property alias gridItemWidth: theGridView.cellWidth

    FWdgtGridView {
        id: theGridView
        anchors.fill: theLayout
        clip: true
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 100
        keyNavigationWraps: listWrap
        flow: GridView.TopToBottom
        snapMode: GridView.SnapOneRow




        populate: Transition {
            id: populateTransition
            SequentialAnimation {
                PropertyAction { property: "opacity"; value: 0 }
                PauseAnimation { duration: populateTransition.ViewTransition.index * 100 }
                NumberAnimation { properties: "opacity"; from: 0; to: 1; duration: 300 }
            }
        }

        add:  Transition {
            NumberAnimation { properties: "opacity"; from: 0; to: 1; duration: 200 }
        }

        addDisplaced: Transition {
            NumberAnimation { properties: "x"; duration: 200 }
        }

        remove:  Transition {
            NumberAnimation { properties: "opacity"; from: 1; to: 0; duration: 200 }
        }

        removeDisplaced: Transition {
            NumberAnimation { properties: "x"; duration: 200 }
        }

    }
}
