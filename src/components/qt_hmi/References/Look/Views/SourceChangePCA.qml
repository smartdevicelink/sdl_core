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
            FMenuElement { id: sourceAM; menuText: "AM"; }
            FMenuElement { id: sourceFM; menuText: "FM"; }
            FMenuElement { id: sourceSat; menuText: "Sirius";  }
            FMenuElement { id: sourceCD; menuText: "CD";  }
            FMenuElement { id: sourceBt; menuText: "BT Audio";   }
            FMenuElement { id: sourceIpod; menuText: "iPod";   }
            FMenuElement { id: sourceUSB; menuText: "USB";   }
            FMenuElement { id: sourceLineIn; menuText: "Line In";}            
            FMenuElement { id: sourceSDL; menuText: "SDL Music";}
        }

        FViewUseCaseSimple { condition: sv.isInEntry
            PropertyChanges { target: l; list_data: null }
        }
    }

    baseBehavior: GenListHandling {}

    triggers: [
        // Example for a menu element trigger. This will only work for FMenuModels.
        FTrgMenuElement { menuElement: sourceAM
            FActScriptCall { onScript: dataPool.currSource = "AM" }
            FActViewShow { view: "RadioEventHandlingPCA" }
        },
        FTrgMenuElement { menuElement: sourceFM
            FActScriptCall { onScript: dataPool.currSource = "FM" }
            FActViewShow { view: "RadioEventHandlingPCA" }
        },
        FTrgMenuElement { menuElement: sourceSat
            FActScriptCall { onScript: dataPool.currSource = "Sirius" }
            FActViewShow { view: "RadioEventHandlingPCA" }
        },
        FTrgMenuElement { menuElement: sourceCD
            FActScriptCall { onScript: dataPool.currSource = "CD" }
            FActViewShow { view: "RadioEventHandlingPCA" }
        },
        FTrgMenuElement { menuElement: sourceBt
            FActScriptCall { onScript: dataPool.currSource = "BT_Audio" }
            FActViewShow { view: "RadioEventHandlingPCA" }
        },
        FTrgMenuElement { menuElement: sourceIpod
            FActScriptCall { onScript: dataPool.currSource = "iPod" }
            FActViewShow { view: "RadioEventHandlingPCA" }
        },
        FTrgMenuElement { menuElement: sourceUSB
            FActScriptCall { onScript: dataPool.currSource = "USB" }
            FActViewShow { view: "RadioEventHandlingPCA" }
        },
        FTrgMenuElement { menuElement: sourceLineIn
            FActScriptCall { onScript: dataPool.currSource = "LineIn" }
            FActViewShow { view: "RadioEventHandlingPCA" }
        },
        FTrgMenuElement { menuElement: sourceSDL
            FActScriptCall { onScript: dataPool.currSource = "SDL_Music" }
            FActViewShow { view: "RadioEventHandlingPCA" }
        },
        FTrgTouchArea { touchArea:l.close; touchAction: TA.Out
            FActViewShow { view: "RadioEventHandlingPCA" }
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
