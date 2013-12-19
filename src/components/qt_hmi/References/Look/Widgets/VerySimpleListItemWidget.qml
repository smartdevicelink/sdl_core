import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"


FWidget {
    id: w
    height: 50
    property alias vis: l.vis

    layout: SimpleItemWdgLayout {
        anchors.fill: parent
        id: l
        cx_text: staticItem
    }

}
