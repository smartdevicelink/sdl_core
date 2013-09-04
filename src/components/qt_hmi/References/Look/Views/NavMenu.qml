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
        layout: TileBackLayoutPCA {
            id:l


            listWrap: false
            list_data: FProxyListModel {
                id: menuProxy
                sourceModel: menuItemData
            }
            list_itemWidget: GridViewItemWidget {

            }
        }

        FMenuModel {
            id: menuItemData
            FMenuElement { id: sourceAM; menuText: "Home"; }
            FMenuElement { id: sourceFM; menuText: "Work"; }
            FMenuElement { id: sourceCD; menuText: "Keyboard";  }
            FMenuElement { id: sourceSat; menuText: "Recent";  }
            FMenuElement { id: sourceBt; menuText: "Favorites";   }
            FMenuElement { id: sourceIpod; menuText: "POI";   }
            FMenuElement { id: sourceUSB; menuText: "Scout";   }
            FMenuElement { id: sourceLineIn; menuText: "Waze";}
        }

        FViewUseCaseSimple { condition: sv.isInEntry
            PropertyChanges { target: l; list_data: null }
        }
    }

    baseBehavior: GenListHandling {}

    triggers: [
        // Example for a menu element trigger. This will only work for FMenuModels.
        FTrgMenuElement { menuElement: sourceAM
        },
        FTrgMenuElement { menuElement: sourceFM
        },
        FTrgMenuElement { menuElement: sourceSat
        },
        FTrgMenuElement { menuElement: sourceCD
            FActViewShow { view: "NavKeyboardPCA" }
        },
        FTrgMenuElement { menuElement: sourceBt
        },
        FTrgMenuElement { menuElement: sourceIpod
        },
        FTrgMenuElement { menuElement: sourceUSB
        },
        FTrgMenuElement { menuElement: sourceLineIn
        },
        FTrgTouchArea { touchArea:l.close; touchAction: TA.Out
            FActViewShow { view: "NavNoRoute" }
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
