// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0

import "../Widgets"

FLayout {
    id: theItem
    height: 50; width: parent.width

    property alias cx_text : cx.text
    property alias cx_width : cx.width
    property alias ax_width : ax.width
    property alias ax_bitmap : ax.bitmap
    property alias activeMarker_visible: activeMarker.visible
    property alias touch: _touch
    property int indentLevel: 0

    Rectangle {
        id: activeMarker
        anchors.fill: theItem
        height: 50
        radius: 10
        color: "lightgray"
    }

    Item {
        x: 1; y: 0
        width: 502; height: 45

        BaseText {
            id: cx
            anchors.fill: parent
            anchors.leftMargin: 20*indentLevel+5
            vertAlignment: Text.AlignVCenter
        }
    }

    FWdgtImage {
        id: ax
        x: 549; y: 3
        width: 37; height: 45
    }

    FWdgtTouchArea {
        id: _touch
        width: 400
        height: 50
    }
}
