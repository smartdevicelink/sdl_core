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


            //useClose: true
            listWrap: false
            list_data: FProxyListModel {
                id: menuProxy
                sourceModel: menuItemData
            }
            list_itemWidget: GridViewImgItemWidget {

            }
        }

        FMenuModel {
            id: menuItemData
            FMenuElement { id: ev; menuIcon: "Vehicle/EV Icon.png"}
            FMenuElement { id: aLight; menuIcon: "Vehicle/Ambient Lighti Icon.png" }
            FMenuElement { id: mcs; menuIcon: "Vehicle/MCS Icon.png"  }
            FMenuElement { id: weather; menuIcon: "Vehicle/sun.png"  }
            FMenuElement { id: kepad; menuIcon: "Vehicle/key_car.png"   }
            FMenuElement { id: calendar; menuIcon: "Vehicle/cal.png"  }
            FMenuElement { id: browse; menuIcon: "Vehicle/folders.png"  }
            FMenuElement { id: syncServices; menuIcon: "Vehicle/SYNC Services.png"}
        }

        FViewUseCaseSimple { condition: sv.isInEntry
            PropertyChanges { target: l; list_data: null }
        }
    }

    baseBehavior: GenListHandling {}

    triggers: [
        // Example for a menu element trigger. This will only work for FMenuModels.
        FTrgMenuElement { menuElement: ev
        },
        FTrgMenuElement { menuElement: aLight
        },
        FTrgMenuElement { menuElement: mcs
        },
        FTrgMenuElement { menuElement: weather
        },
        FTrgMenuElement { menuElement: kepad
        },
        FTrgMenuElement { menuElement: calendar
        },
        FTrgMenuElement { menuElement: browse
        },
        FTrgMenuElement { menuElement: syncServices
        },
        FTrgTouchArea { touchArea:l.close; touchAction: TA.Out
            FActViewShow { view: "GridMenuPCA" }
        },
        FTrgListModel { id: trgList; model: menuItemData
            FActScriptCall { onScript: { FLogger.debug("Test", trgList.element.menuText + ", index: " + trgList.index) } }
        },
        FTrgEntry {
            FActScriptCall { onScript: { FLogger.debug("Test", "Entry") } }
        },
        FTrgEntryComplete {
            FActScriptCall { onScript: { FLogger.debug("Test", "EntryComplete") } }
        },
        FTrgExit {
            FActScriptCall { onScript: { FLogger.debug("Test", "Exit") } }
        },
        FTrgExitComplete {
            FActScriptCall { onScript: { FLogger.debug("Test", "ExitComplete") } }
        }
    ]
}
