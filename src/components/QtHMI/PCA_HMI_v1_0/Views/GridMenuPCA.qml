// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import com.ford.hmiframework 1.0
import "../Layouts"
import "../Animations"
import "../Widgets"

FStateView {
    id:sv
    priority: 2

    view: FView {
        //viewClass: baseScreen
        layout: TileBackImgLayoutPCA {
            id:l
            itemWidth: 266



            listWrap: false
            list_data: FProxyListModel {
                id: menuProxy
                sourceModel: menuItemData
            }
            list_itemWidget: GridViewImgItemWidget {
                width: 266
            }
        }

        FMenuModel {
            id: menuItemData
            FMenuElement { id: climate;  menuIcon: "menu/snow.png"}
            FMenuElement { id: phone; menuIcon: "menu/phone.png" }
            FMenuElement { id: navigation; menuIcon: "menu/arrow.png"  }
            FMenuElement { id: myVeh; menuIcon: "menu/car.png"  }
            FMenuElement { id: entertainment; menuIcon: "menu/notes.png"   }
            FMenuElement { id: settings; menuIcon: "menu/gear.png"  }
        }

        FViewUseCaseSimple { condition: sv.isInEntry
            PropertyChanges { target: l; list_data: null }
        }
    }

    baseBehavior: GenListHandling {}

    triggers: [
        // Example for a menu element trigger. This will only work for FMenuModels.
        FTrgMenuElement { menuElement: climate
            FActViewShow { view: "ClimatePCA" }
        },
        FTrgMenuElement { menuElement: phone
            FActViewShow { view: "PhonePCA" }
        },
        FTrgMenuElement { menuElement: navigation
            FActViewShow { view: "NavNoRoute" }
        },
        FTrgMenuElement { menuElement: myVeh
            FActViewShow { view: "MyVehiclePCA" }
        },
        FTrgMenuElement { menuElement: entertainment
            FActViewShow { view: "RadioEventHandlingPCA" }
        },
        FTrgMenuElement { menuElement: settings
        },
        FTrgListModel { id: trgList; model: menuItemData
            FActScriptCall { onScript: { FLogger.debug("Test", trgList.element.menuText + ", index: " + trgList.index) } }
        },
        FTrgEntry {
            FActScriptCall { onScript: dataPool.mainPCAmenuShown = true }
            FActScriptCall { onScript: { FLogger.debug("Test", "Entry") } }
        },
        FTrgEntryComplete {
            FActScriptCall { onScript: { FLogger.debug("Test", "EntryComplete") } }
        },
        FTrgExit {
            FActScriptCall { onScript: dataPool.mainPCAmenuShown = false }
            FActScriptCall { onScript: { FLogger.debug("Test", "Exit") } }
        },
        FTrgExitComplete {
            FActScriptCall { onScript: { FLogger.debug("Test", "ExitComplete") } }
        },
        FTrgEvent { event: closeNormalScreens
            FActViewHide{}
        }
    ]
}
