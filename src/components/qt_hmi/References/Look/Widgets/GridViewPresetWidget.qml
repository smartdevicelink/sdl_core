// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../WidgetLayouts"

FWidget {

    property alias selectElement: _selectElement
    property alias list: l.list
    property alias list_itemWidget: l.list_itemWidget
    property alias list_data: l.list_data
    property alias listWrap: l.listWrap
    property alias gridItemHeight: l.gridItemHeight
    property alias gridItemWidth: l.gridItemWidth
    property alias gridCurItem: l.curItem
    property alias moving: l.moving
    property alias direction: l.direction


    FEvent { id: _selectElement; }

    layout: GridViewPresetLayout {
        id: l
        anchors.fill: parent
    }

    triggers: [
        FTrgEvent { event: _selectElement
            FActScriptCall { onScript: {
                    list_data.selectElement(l.list.currentIndex);
                }
            }
        }
    ]
}
