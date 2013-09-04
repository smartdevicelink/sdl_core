import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Widgets"
import "../WidgetLayouts"



FWidget {
    id:sv
    property int currPreset: 2
    property alias listModel: l.list_data
    property alias gridCurItem: l.gridCurItem
    property alias moving: l.moving
    property alias direction: l.direction


    layout: PresetGridLayoutPCA {
        id:l


        listWrap: false
        list_data: FProxyListModel {
            id: menuProxy
            sourceModel: menuItemData
        }
        list_itemWidget: GridViewPresetItemWidget {
            currPreset: sv.currPreset
        }
    }

    FMenuModel {
        id: menuItemData
        FMenuElement { id: preset1; menuText: "96.3"; menuData: false; }
        FMenuElement { id: preset2; menuText: "107.9"; menuData: true }
        FMenuElement { id: preset3; menuText: "104.3"; menuData: false }
        FMenuElement { id: preset4; menuText: "101.9"; menuData: false }
        FMenuElement { id: preset5; menuText: "97.1";  menuData: false }
        FMenuElement { id: preset6; menuText: "91.7"; menuData: false  }
        FMenuElement { id: preset7; menuText: "101.1"; menuData: false  }
        FMenuElement { id: preset8; menuText: "91.9"; menuData: false}
    }

    FViewUseCaseSimple { condition: sv.isInEntry
        PropertyChanges { target: l; list_data: null }
    }


}
